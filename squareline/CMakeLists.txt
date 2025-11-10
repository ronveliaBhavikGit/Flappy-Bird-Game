SET(SOURCES screens/ui_gameScreen.c
    ui.c
    components/ui_comp_hook.c
    ui_helpers.c
    images/ui_img_background_png.c
    images/ui_img_pillard0_png.c
    images/ui_img_pillaru0_png.c
    images/ui_img_pillard1_png.c
    images/ui_img_pillaru1_png.c
    images/ui_img_pillard2_png.c
    images/ui_img_pillaru2_png.c
    images/ui_img_flappyup_png.c
    images/ui_img_flappy_png.c
    images/ui_img_flappygametext_png.c)

add_library(ui ${SOURCES})
