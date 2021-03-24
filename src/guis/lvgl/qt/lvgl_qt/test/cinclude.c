typedef union {
    int num;
    const void* ptr;
}lv_style_value_t;

lv_style_value_t v = {
    .num = 1
};