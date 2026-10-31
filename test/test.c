#include <stdio.h>

#include "sadlib.h"

// Tests

#include "core.h"

#include "array.h"
#include "string.h"
#include "unicode.h"
#include "base64.h"
#include "fmap.h"
#include "fdict.h"
#include "sdict.h"

#include "int.h"
#include "float.h"
#include "vec2.h"
#include "vec3.h"
#include "vec4.h"
#include "quat.h"
#include "mat3.h"
#include "mat4.h"

int main(int argc, const char** argv)
{
    sad_wnd* window = NULL;

    if (test_core() != SAD_OK)
        return 0;
    if (test_array() != SAD_OK)
        return 0;
    if (test_string() != SAD_OK)
        return 0;
    if (test_unicode() != SAD_OK)
        return 0;
    if (test_base64() != SAD_OK)
        return 0;
    if (test_fmap() != SAD_OK)
        return 0;
    if (test_fdict() != SAD_OK)
        return 0;
    if (test_sdict() != SAD_OK)
        return 0;

    
}