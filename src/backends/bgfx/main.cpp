
#include "precomp.h"
#include "main.h"
//#include "Sirelphy/source/interface/universe_interface.h"
//#include "Sirelphy/source/interface/object_interface.h"
//#include "Sirelphy/source/sirelphy.h"

#define RENDER_PASS_SHADING 0  // Default forward rendered geo with simple shading
#define RENDER_PASS_ID      1  // ID buffer for picking
#define RENDER_PASS_BLIT    2  // Blit GPU render target to CPU texture

#define ID_DIM 8  // Size of the ID buffer

struct PosColorVertex
{
	float m_x;
	float m_y;
	float m_z;
	uint32_t m_abgr;

	static void init()
	{
		ms_layout
			.begin()
			.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
			.add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
			.end();
	};

	static bgfx::VertexLayout ms_layout;
};

bgfx::VertexLayout PosColorVertex::ms_layout;

static PosColorVertex s_cubeVertices[] =
{
	{-1.0f,  1.0f,  1.0f, 0xff000000 },
	{ 1.0f,  1.0f,  1.0f, 0xff0000ff },
	{-1.0f, -1.0f,  1.0f, 0xff00ff00 },
	{ 1.0f, -1.0f,  1.0f, 0xff00ffff },
	{-1.0f,  1.0f, -1.0f, 0xffff0000 },
	{ 1.0f,  1.0f, -1.0f, 0xffff00ff },
	{-1.0f, -1.0f, -1.0f, 0xffffff00 },
	{ 1.0f, -1.0f, -1.0f, 0xffffffff },
};

static const uint16_t s_cubeTriList[] =
{
	0, 1, 2, // 0
	1, 3, 2,
	4, 6, 5, // 2
	5, 6, 7,
	0, 2, 4, // 4
	4, 2, 6,
	1, 5, 3, // 6
	5, 7, 3,
	0, 4, 1, // 8
	4, 5, 1,
	2, 3, 6, // 10
	6, 3, 7,
};

static const uint16_t s_cubeTriStrip[] =
{
	0, 1, 2,
	3,
	7,
	1,
	5,
	0,
	4,
	2,
	6,
	7,
	4,
	5,
};

static const uint16_t s_cubeLineList[] =
{
	0, 1,
	0, 2,
	0, 4,
	1, 3,
	1, 5,
	2, 3,
	2, 6,
	3, 7,
	4, 5,
	4, 6,
	5, 7,
	6, 7,
};

static const uint16_t s_cubeLineStrip[] =
{
	0, 2, 3, 1, 5, 7, 6, 4,
	0, 2, 6, 4, 5, 7, 3, 1,
	0,
};

static const uint16_t s_cubePoints[] =
{
	0, 1, 2, 3, 4, 5, 6, 7
};




cManager * cManager::managerPtr = 0;

cManager::cManager() 
: m_pt(0)
, m_r(true)
, m_g(true)
, m_b(true)
, m_a(true)
{}

cManager* cManager::get() {
	#ifdef DEBUG
	if (!managerPtr) throw_line("Manager pointer not set");
	#endif // DEBUG
	return managerPtr;
}

void cManager::init(int32_t _argc, const char* const* _argv, uint32_t _width, uint32_t _height, entry::AppI* _appPtr) {
	#ifdef DEBUG
	if (managerPtr) throw_line("Manager pointer already set");
	#endif // DEBUG
	managerPtr = new cManager();
	managerPtr->_init(_argc, _argv, _width, _height, _appPtr);
}

void cManager::_init(int32_t _argc, const char* const* _argv, uint32_t _width, uint32_t _height, entry::AppI* _appPtr) {
	
	//fnSirelphy();

	//uni_interface = new cUniverseInterface;

	//uni_interface->launchTernaryPhoton(oiA, oiB1, oiB2);

	appPtr = _appPtr;

	Args args(_argc, _argv);

	m_width = _width;
	m_height = _height;
	m_debug = BGFX_DEBUG_NONE;
	m_reset = BGFX_RESET_VSYNC;

	bgfx::Init init;
	init.type = args.m_type;
	init.vendorId = args.m_pciId;
	init.resolution.width = m_width;
	init.resolution.height = m_height;
	init.resolution.reset = m_reset;
	bgfx::init(init);

	// Enable debug text.
	bgfx::setDebug(m_debug);

	// Set view 0 clear state.
	bgfx::setViewClear(0
		, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH
		, 0x303030ff
		, 1.0f
		, 0
	);

	// ID buffer clears to black, which represnts clicking on nothing (background)
	bgfx::setViewClear(RENDER_PASS_ID
		, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH
		, 0x000000ff
		, 1.0f
		, 0
	);

	// Create uniforms
	u_tint = bgfx::createUniform("u_tint", bgfx::UniformType::Vec4); // Tint for when you click on items
	u_id = bgfx::createUniform("u_id", bgfx::UniformType::Vec4); // ID for drawing into ID buffer

	// Create program from shaders.
	m_shadingProgram = loadProgram("vs_picking_shaded", "fs_picking_shaded"); // Blinn shading
	m_idProgram = loadProgram("vs_picking_shaded", "fs_picking_id");     // Shader for drawing into ID buffer

	static const char* meshPaths[] =
	{
		"meshes/orb.bin",
		"meshes/column.bin",
		"meshes/bunny.bin",
		"meshes/cube.bin",
		"meshes/tree.bin",
		"meshes/hollowcube.bin",
	};

	static const float meshScale[] =
	{
		0.5f,
		0.05f,
		0.5f,
		0.25f,
		0.05f,
		0.05f,
	};

	m_highlighted = UINT32_MAX;

	// Set up ID buffer, which has a color target and depth buffer
	m_pickingRT = bgfx::createTexture2D(ID_DIM, ID_DIM, false, 1, bgfx::TextureFormat::RGBA8, 0
		| BGFX_TEXTURE_RT
		| BGFX_SAMPLER_MIN_POINT
		| BGFX_SAMPLER_MAG_POINT
		| BGFX_SAMPLER_MIP_POINT
		| BGFX_SAMPLER_U_CLAMP
		| BGFX_SAMPLER_V_CLAMP
	);
	m_pickingRTDepth = bgfx::createTexture2D(ID_DIM, ID_DIM, false, 1, bgfx::TextureFormat::D24S8, 0
		| BGFX_TEXTURE_RT
		| BGFX_SAMPLER_MIN_POINT
		| BGFX_SAMPLER_MAG_POINT
		| BGFX_SAMPLER_MIP_POINT
		| BGFX_SAMPLER_U_CLAMP
		| BGFX_SAMPLER_V_CLAMP
	);

	// CPU texture for blitting to and reading ID buffer so we can see what was clicked on.
	// Impossible to read directly from a render target, you *must* blit to a CPU texture
	// first. Algorithm Overview: Render on GPU -> Blit to CPU texture -> Read from CPU
	// texture.
	m_blitTex = bgfx::createTexture2D(ID_DIM, ID_DIM, false, 1, bgfx::TextureFormat::RGBA8, 0
		| BGFX_TEXTURE_BLIT_DST
		| BGFX_TEXTURE_READ_BACK
		| BGFX_SAMPLER_MIN_POINT
		| BGFX_SAMPLER_MAG_POINT
		| BGFX_SAMPLER_MIP_POINT
		| BGFX_SAMPLER_U_CLAMP
		| BGFX_SAMPLER_V_CLAMP
	);

	bgfx::TextureHandle rt[2] =
	{
		m_pickingRT,
		m_pickingRTDepth
	};
	m_pickingFB = bgfx::createFrameBuffer(BX_COUNTOF(rt), rt, true);

//	cUniverseInterface universeInterface;

	_init_cubes();
	m_timeOffset = bx::getHPCounter();

	imguiCreate();
}

void cManager::_init_cubes() {
	// Create vertex stream declaration.
	PosColorVertex::init();

	// Create static vertex buffer.
	m_vbh = bgfx::createVertexBuffer(
		// Static data can be passed with bgfx::makeRef
		bgfx::makeRef(s_cubeVertices, sizeof(s_cubeVertices))
		, PosColorVertex::ms_layout
	);

	// Create static index buffer for triangle list rendering.
	m_ibh[0] = bgfx::createIndexBuffer(
		// Static data can be passed with bgfx::makeRef
		bgfx::makeRef(s_cubeTriList, sizeof(s_cubeTriList))
	);

	// Create static index buffer for triangle strip rendering.
	m_ibh[1] = bgfx::createIndexBuffer(
		// Static data can be passed with bgfx::makeRef
		bgfx::makeRef(s_cubeTriStrip, sizeof(s_cubeTriStrip))
	);

	// Create static index buffer for line list rendering.
	m_ibh[2] = bgfx::createIndexBuffer(
		// Static data can be passed with bgfx::makeRef
		bgfx::makeRef(s_cubeLineList, sizeof(s_cubeLineList))
	);

	// Create static index buffer for line strip rendering.
	m_ibh[3] = bgfx::createIndexBuffer(
		// Static data can be passed with bgfx::makeRef
		bgfx::makeRef(s_cubeLineStrip, sizeof(s_cubeLineStrip))
	);

	// Create static index buffer for point list rendering.
	m_ibh[4] = bgfx::createIndexBuffer(
		// Static data can be passed with bgfx::makeRef
		bgfx::makeRef(s_cubePoints, sizeof(s_cubePoints))
	);

	// Create program from shaders.
	m_program = loadProgram("vs_cubes", "fs_cubes");
}

bool cManager::update() {

	// Update

	//uni_interface->advanceTime(0.0);

	_update_imgui();

	if (!entry::processEvents(m_width, m_height, m_debug, m_reset, &m_mouseState))
	{


		//			float time = (float)( (bx::getHPCounter()-m_timeOffset)/double(bx::getHPFrequency() ) );

		const bx::Vec3 at = { 0.0f, 0.0f,   0.0f };
		const bx::Vec3 eye = { 0.0f, 0.0f, -35.0f };

		// Set view and projection matrix for view 0.
		{
			float view[16];
			bx::mtxLookAt(view, eye, at);

			float proj[16];
			bx::mtxProj(proj, 60.0f, float(m_width) / float(m_height), 0.1f, 100.0f, bgfx::getCaps()->homogeneousDepth);
			bgfx::setViewTransform(0, view, proj);

			// Set view 0 default viewport.
			bgfx::setViewRect(0, 0, 0, uint16_t(m_width), uint16_t(m_height));
		}

		// This dummy draw call is here to make sure that view 0 is cleared
		// if no other draw calls are submitted to view 0.
		bgfx::touch(0);

		bgfx::IndexBufferHandle ibh = m_ibh[m_pt];
		uint64_t state = 0
			| (m_r ? BGFX_STATE_WRITE_R : 0)
			| (m_g ? BGFX_STATE_WRITE_G : 0)
			| (m_b ? BGFX_STATE_WRITE_B : 0)
			| (m_a ? BGFX_STATE_WRITE_A : 0)
			| BGFX_STATE_WRITE_Z
			| BGFX_STATE_DEPTH_TEST_LESS
			| BGFX_STATE_CULL_CW
			| BGFX_STATE_MSAA
			| s_ptState[m_pt]
			;

		// Submit 11x11 cubes.
		for (uint32_t yy = 0; yy < 11; ++yy)
		{
			for (uint32_t xx = 0; xx < 11; ++xx)
			{
				for (uint32_t zz = 0; zz < 11; ++zz)
				{

					float mtx[16];
					bx::mtxRotateXY(mtx, 0.0f, 0.0f);
					//bx::mtxRotateXY(mtx, time + xx * 0.21f, time + yy * 0.37f);
					mtx[12] = -15.0f + float(xx) * 2.0f;
					mtx[13] = -15.0f + float(yy) * 2.0f;
					mtx[14] = float(zz) * 2.0f;// -15.0f + float(zz) * 2.0f;

					// Set model matrix for rendering.
					bgfx::setTransform(mtx);

					// Set vertex and index buffer.
					bgfx::setVertexBuffer(0, m_vbh);
					bgfx::setIndexBuffer(ibh);

					// Set render states.
					bgfx::setState(state);

					// Submit primitive for rendering to view 0.
					bgfx::submit(0, m_program);
				}
			}
		}

		// Advance to next frame. Rendering thread will be kicked to
		// process submitted rendering primitives.
		bgfx::frame();

		return true;
	}

	return false;
}

void cManager::_update_imgui() {
	imguiBeginFrame(m_mouseState.m_mx
		, m_mouseState.m_my
		, (m_mouseState.m_buttons[entry::MouseButton::Left] ? IMGUI_MBUT_LEFT : 0)
		| (m_mouseState.m_buttons[entry::MouseButton::Right] ? IMGUI_MBUT_RIGHT : 0)
		| (m_mouseState.m_buttons[entry::MouseButton::Middle] ? IMGUI_MBUT_MIDDLE : 0)
		, m_mouseState.m_mz
		, uint16_t(m_width)
		, uint16_t(m_height)
	);

	showExampleDialog(appPtr);

	ImGui::SetNextWindowPos(
		ImVec2(m_width - m_width / 5.0f - 10.0f, 10.0f)
		, ImGuiCond_FirstUseEver
	);
	ImGui::SetNextWindowSize(
		ImVec2(m_width / 5.0f, m_height / 3.5f)
		, ImGuiCond_FirstUseEver
	);
	ImGui::Begin("Settings"
		, NULL
		, 0
	);

	ImGui::Checkbox("Write R", &m_r);
	ImGui::Checkbox("Write G", &m_g);
	ImGui::Checkbox("Write B", &m_b);
	ImGui::Checkbox("Write A", &m_a);

	double velA = 5.0;
	//oiA->getVelocityMagnitudeC(velA);
	std::string velAstr = "Velocity (A): " + std::to_string(velA);
	ImGui::Text(velAstr.c_str());

	//const double calcSpeedLight = (double)speedOfLight * (double)units_per_second / (double)units_per_meter;
	//std::string speedStr = "Calculated speed of light: " + std::to_string(calcSpeedLight);
	//ImGui::Text(speedStr.c_str());

	//const double biggestint = (double)(std::numeric_limits<bigint>::max());
	//const double maxSize = biggestint / (double)units_per_meter;
	//const double maxAge = biggestint / (double)units_per_second;

	//std::string maxUniverseSize = "Max. universe size: " + std::to_string(uint64_t((maxSize / calcSpeedLight)/1000000000)) + " billion light years";
	//std::string maxUniverseAge = "Max. universe lifespan: " + std::to_string(uint64_t(maxAge / 31557600000000000)) + " billion years";
	//ImGui::Text(maxUniverseSize.c_str());
	//ImGui::Text(maxUniverseAge.c_str());

	//std::string speedLightCheck = "Speed of light (fundamental units): " + std::to_string(uint64_t(((double)speed_of_light_fund_units_check))) + " units";
	//ImGui::Text(speedLightCheck.c_str());
	//
	//std::string lowSpeedCheck = "Lowest measurable speed: " + std::to_string((((double)lowest_speed_check))) + " (m/s)";
	//ImGui::Text(lowSpeedCheck.c_str());

	ImGui::Text("Primitive topology:");
	ImGui::Combo("", (int*)&m_pt, s_ptNames, BX_COUNTOF(s_ptNames));

	ImGui::SliderFloat("Field of view", &m_fov, 1.0f, 60.0f);

	ImGui::End();

	imguiEndFrame();
}

int cManager::shutdown() {
	imguiDestroy();

	// Cleanup.
	for (uint32_t ii = 0; ii < BX_COUNTOF(m_ibh); ++ii)
	{
		bgfx::destroy(m_ibh[ii]);
	}

	bgfx::destroy(m_vbh);
	bgfx::destroy(m_program);


	for (uint32_t ii = 0; ii < 12; ++ii)
	{
		meshUnload(m_meshes[ii]);
	}

	// Cleanup.
	bgfx::destroy(m_shadingProgram);
	bgfx::destroy(m_idProgram);

	bgfx::destroy(u_tint);
	bgfx::destroy(u_id);

	bgfx::destroy(m_pickingFB);
	bgfx::destroy(m_pickingRT);
	bgfx::destroy(m_pickingRTDepth);
	bgfx::destroy(m_blitTex);

	// Shutdown bgfx.
	bgfx::shutdown();

	return 0;
}