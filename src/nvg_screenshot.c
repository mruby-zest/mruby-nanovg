#include <mruby.h>
#include <stdint.h>
#include "gl_core.3.2.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../../nanovg/example/stb_image_write.h"



static void flipHorizontal(unsigned char* image, int w, int h, int stride)
{
    int i = 0, j = h-1, k;
    while (i < j) {
        unsigned char* ri = &image[i * stride];
        unsigned char* rj = &image[j * stride];
        for (k = 0; k < w*4; k++) {
            unsigned char t = ri[k];
            ri[k] = rj[k];
            rj[k] = t;
        }
        i++;
        j--;
    }
}

    mrb_value
nvg_screenshot(mrb_state *mrb, mrb_value self)
{
    int x, y, w, h;
    const char *fname;
    mrb_get_args(mrb, "iiiiz",
            &x, &y, &w, &h, &fname);

    uint8_t *image = (uint8_t*)calloc(w*h,4);

    //uint32_t sum = 0;
    glReadPixels(x, y, w, h,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            image);
    int ret = glGetError();
    if(ret != 0)
        printf("OPENGL ERROR %d\n", ret);
    //for(int i=0; i<4*w*h; ++i)
    //    sum += image[i];
    //printf("sum = %d\n", sum);

    flipHorizontal(image, w, h, w*4);

    stbi_write_png(fname, w, h, 4, image, w*4);
    free(image);
    return self;
}
