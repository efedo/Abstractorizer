#pragma once

#include "common.h"
#include "bgfx_utils.h"
#include "imgui/imgui.h"

#include "precomp.h"

class cUniverseInterface;
class cObjectInterface;

// Global max side length before subdivision (in m)
const double fidelity = 2.0;
const uint8_t maxSubdivLevel = 10;

// Required fidelity at a given subdivision level
//const double fidelityMultiplePerLevel = 0.5;

// type: continuous, disjointed




// Can glue a vertex to a face, to a point on an edge, or to another vertex
// (with or without freedom to rotate?)

// Can glue part of an edge to a face, part of an edge, or a vertex

// Can glue part of an edge to part of an edge

class cObject;


// Note that continuous meshes should usually be joined through shared vertices rather than glued connections

// WHen you fill full blocks, should arrange according to global axes for ease of calculations


enum class tGlueConnectionType : uint8_t { full_face = 0 };

class cGluedConnection {
public:
private:
	cObject* objA;
	cObject* objB;
	// object A
		// face
	// object B
		// face
	double strength = 5.0; // Required force to break the bond
};

// General rules:
// 1. All non-planar blop sides are assumed to have an implicit center point (at the exact average position),
//    and can subdivide into four triangles

// Parent rules:
// 1. Must be Hexahedron (i.e. six-sided) [it is possible to create heptahedrons and octahedrons with 8 vertices]
//   a. enforce planar quads

// Child rules:
// 1. Children can not overlap
// 2. Children can not extend beyond the edges of the parent
// 3. Summed actual volume of children should equal the actual volume of parent, and be less than the
//		simplified volume of the parent



// Make object x at position a, b, c, relative to my current position

// cViewerPosition(cFrame * myframe, cVec3D_big relpos);

// cObject







// Want to have accurate positions of: bins/frames
// Viewer
// Object positions within a frame can be relative to frame


const uint8_t subdivideWhen = 4; // Subdivide bins with more than this number of direct children
const double maxPosition = 10000000; // Maximum distance from origin for any object (in m); 
									// limited to 10,000 km for performance and to reduce FP error
									// beyond this will have to use quads or double doubles

// There will be a separate detailed graph constructed from the current/nearby bins



static const char* s_ptNames[]
{
	"Triangle List",
	"Triangle Strip",
	"Lines",
	"Line Strip",
	"Points",
};

static const uint64_t s_ptState[]
{
	UINT64_C(0),
	BGFX_STATE_PT_TRISTRIP,
	BGFX_STATE_PT_LINES,
	BGFX_STATE_PT_LINESTRIP,
	BGFX_STATE_PT_POINTS,
};

BX_STATIC_ASSERT(BX_COUNTOF(s_ptState) == BX_COUNTOF(s_ptNames));

class cManager {
public:
	static cManager* get();
	static void init(int32_t, const char* const*, uint32_t, uint32_t, entry::AppI*);
	bool update();
	int shutdown();
private:
	void _init(int32_t, const char* const*, uint32_t, uint32_t, entry::AppI*);
	void _init_cubes();
	void _update_imgui();

	cManager::cManager();
	static cManager* managerPtr;

	entry::AppI* appPtr = 0;
	
	entry::MouseState m_mouseState;

	uint32_t m_width;
	uint32_t m_height;
	uint32_t m_debug;
	uint32_t m_reset;
	bgfx::VertexBufferHandle m_vbh;
	bgfx::IndexBufferHandle m_ibh[BX_COUNTOF(s_ptState)];
	bgfx::ProgramHandle m_program;
	int64_t m_timeOffset;
	int32_t m_pt;

	bool m_r;
	bool m_g;
	bool m_b;
	bool m_a;


	Mesh* m_meshes[12];
	float m_meshScale[12];
	float m_idsF[12][4];
	uint32_t m_idsU[12];
	uint32_t m_highlighted;

	// Resource handles
	bgfx::ProgramHandle m_shadingProgram;
	bgfx::ProgramHandle m_idProgram;
	bgfx::UniformHandle u_tint;
	bgfx::UniformHandle u_id;
	bgfx::TextureHandle m_pickingRT;
	bgfx::TextureHandle m_pickingRTDepth;
	bgfx::TextureHandle m_blitTex;
	bgfx::FrameBufferHandle m_pickingFB;

	float m_fov;


	// phys stuff
	cUniverseInterface * uni_interface;

	cObjectInterface* oiA = 0;
	cObjectInterface* oiB1 = 0;
	cObjectInterface* oiB2 = 0;
};

