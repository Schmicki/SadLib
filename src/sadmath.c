#include <math.h>
#include "sadmath.h"

/*
* sad_int
* ----------------------------------------------------------------------------------------------- */

sad_int sad_iabs(sad_int v)
{
    return abs(v);
}

sad_int sad_imin(sad_int a, sad_int b)
{
    return min(a, b);
}

sad_int sad_imax(sad_int a, sad_int b)
{
    return max(a, b);
}

sad_int sad_iclamp(sad_int v, sad_int min, sad_int max)
{
    return sad_imax(min, sad_imin(max, v));
}

sad_int sad_ipow(sad_int v, sad_int exp)
{
    return (sad_int)sad_fpow((sad_float)v, (sad_float)exp);
}

sad_uint sad_umin(sad_uint a, sad_uint b)
{
    return min(a, b);
}

sad_uint sad_umax(sad_uint a, sad_uint b)
{
    return max(a, b);
}

sad_uint sad_uclamp(sad_uint v, sad_uint min, sad_uint max)
{
    return sad_umax(min, sad_umin(max, v));
}

sad_uint sad_upow(sad_uint v, sad_uint exp)
{
    return (sad_uint)sad_fpow((sad_float)v, (sad_float)exp);
}

/*
* sad_float
* ----------------------------------------------------------------------------------------------- */

#ifdef SAD_FLOAT_64

sad_float sad_fabs(sad_float v)
{
    return fabs(v);
}

sad_float sad_fmin(sad_float a, sad_float b)
{
    return fmin(a, b);
}

sad_float sad_fmax(sad_float a, sad_float b)
{
    return fmax(a, b);
}

sad_float sad_ftrunc(sad_float v)
{
    return trunc(v);
}

sad_float sad_fround(sad_float v)
{
    return round(v);
}

sad_float sad_fmod(sad_float v, sad_float mod)
{
    return fmod(v, mod);
}

sad_float sad_fclamp(sad_float v, sad_float min, sad_float max)
{
    return fmax(min, fminf(max, v));
}

sad_int sad_fisclose(sad_float a, sad_float b, sad_float epsilon)
{
    if (a == b)
        return 1;

    if (isinf(a) || isinf(b))
        return 0;

    return sad_fabs(a - b) <= sad_fmax(epsilon * sad_fmax(a, b), 0.000000000001);
}

sad_float sad_fsin(sad_float v)
{
    return sin(v);
}

sad_float sad_fasin(sad_float v)
{
    return asin(v);
}

sad_float sad_fcos(sad_float v)
{
    return cos(v);
}

sad_float sad_facos(sad_float v)
{
    return acos(v);
}

sad_float sad_ftan(sad_float v)
{
    return tan(v);
}

sad_float sad_fatan(sad_float v)
{
    return atan(v);
}

sad_float sad_fsqrt(sad_float v)
{
    return sqrt(v);
}

sad_float sad_fpow(sad_float v, sad_float exp)
{
    return pow(v, exp);
}

sad_float sad_flog2(sad_float v)
{
    return log2(v);
}

sad_int sad_fcmp(sad_float a, sad_float b)
{
    return (a != b) * (a > b ? 1 : -1);
}

#else

sad_float sad_fabs(sad_float v)
{
    return fabsf(v);
}

sad_float sad_fmin(sad_float a, sad_float b)
{
    return fminf(a, b);
}

sad_float sad_fmax(sad_float a, sad_float b)
{
    return fmaxf(a, b);
}

sad_float sad_ftrunc(sad_float v)
{
    return truncf(v);
}

sad_float sad_fround(sad_float v)
{
    return roundf(v);
}

sad_float sad_fmod(sad_float v, sad_float mod)
{
    return fmodf(v, mod);
}

sad_float sad_fclamp(sad_float v, sad_float min, sad_float max)
{
    return fmaxf(min, fminf(max, v));
}

sad_int sad_fisclose(sad_float a, sad_float b, sad_float epsilon)
{
    if (a == b)
        return 1;

    if (isinf(a) || isinf(b))
        return 0;

    return sad_fabs(a - b) <= sad_fmax(epsilon * sad_fmax(a, b), 0.0000001f);
}

sad_float sad_fsin(sad_float v)
{
    return sinf(v);
}

sad_float sad_fasin(sad_float v)
{
    return asinf(v);
}

sad_float sad_fcos(sad_float v)
{
    return cosf(v);
}

sad_float sad_facos(sad_float v)
{
    return acosf(v);
}

sad_float sad_ftan(sad_float v)
{
    return tanf(v);
}

sad_float sad_fatan(sad_float v)
{
    return atanf(v);
}

sad_float sad_fsqrt(sad_float v)
{
    return sqrtf(v);
}

sad_float sad_fpow(sad_float v, sad_float exp)
{
    return powf(v, exp);
}

sad_float sad_flog2(sad_float v)
{
    return log2f(v);
}

sad_int sad_fcmp(sad_float a, sad_float b)
{
    return (a != b) * (a > b ? 1 : -1);
}

#endif

/*
* sad_vec2
* ----------------------------------------------------------------------------------------------- */

sad_vec2 *sad_vec2_set(sad_vec2 *dst, sad_float x, sad_float y)
{
    dst->x = x;
    dst->y = y;
    return dst;
}

sad_vec2 *sad_vec2_add(sad_vec2 *dst, sad_vec2 *b)
{
    dst->x += b->x;
    dst->y += b->y;
    return dst;
}

sad_vec2 *sad_vec2_sub(sad_vec2 *dst, sad_vec2 *b)
{
    dst->x -= b->x;
    dst->y -= b->y;
    return dst;
}

sad_vec2 *sad_vec2_mul(sad_vec2 *dst, sad_float v)
{
    dst->x *= v;
    dst->y *= v;
    return dst;
}

sad_vec2 *sad_vec2_div(sad_vec2 *dst, sad_float v)
{
    dst->x /= v;
    dst->y /= v;
    return dst;
}

sad_float sad_vec2_dot(sad_vec2 *a, sad_vec2 *b)
{
    return a->x * b->x + a->y * b->y;
}

sad_float sad_vec2_len(sad_vec2 *src)
{
    return sad_fsqrt(sad_vec2_dot(src, src));
}

sad_vec2 *sad_vec2_unit(sad_vec2 *dst)
{
    sad_float length = sad_vec2_len(dst);
    return sad_vec2_div(dst, length);
}

sad_vec2 *sad_vec2_lerp(sad_vec2 *from_dst, sad_vec2 *to, sad_float alpha)
{
    sad_vec2 step = *to;
    sad_vec2_mul(sad_vec2_sub(&step, from_dst), alpha);
    return sad_vec2_add(from_dst, &step);
}

sad_int sad_vec2_cmp(sad_vec2 *a, sad_vec2 *b)
{
    if (a->x != b->x) return a->x > b->x ? 1 : -1;
    if (a->y != b->y) return a->y > b->y ? 1 : -1;
    return 0;
}

/*
* sad_vec3
* ----------------------------------------------------------------------------------------------- */

sad_vec3* sad_vec3_set(sad_vec3* dst, sad_float x, sad_float y, sad_float z)
{
    dst->x = x;
    dst->y = y;
    dst->z = z;
    return dst;
}

sad_vec3* sad_vec3_add(sad_vec3* dst, sad_vec3* b)
{
    dst->x += b->x;
    dst->y += b->y;
    dst->z += b->z;
    return dst;
}

sad_vec3* sad_vec3_sub(sad_vec3* dst, sad_vec3* b)
{
    dst->x -= b->x;
    dst->y -= b->y;
    dst->z -= b->z;
    return dst;
}

sad_vec3* sad_vec3_mul(sad_vec3* dst, sad_float v)
{
    dst->x *= v;
    dst->y *= v;
    dst->z *= v;
    return dst;
}

sad_vec3* sad_vec3_div(sad_vec3* dst, sad_float v)
{
    dst->x /= v;
    dst->y /= v;
    dst->z /= v;
    return dst;
}

sad_float sad_vec3_dot(sad_vec3* a, sad_vec3* b)
{
    return a->x * b->x + a->y * b->y + a->z * b->z;
}

sad_vec3* sad_vec3_cross(sad_vec3* dst, sad_vec3* b)
{
    sad_vec3 a = *dst;
    dst->x = (a.y * b->z) - (a.z * b->y);
    dst->y = (a.z * b->x) - (a.x * b->z);
    dst->z = (a.x * b->y) - (a.y * b->x);
    return dst;
}

sad_float sad_vec3_len(sad_vec3* src)
{
    return sad_fsqrt(sad_vec3_dot(src, src));
}

sad_vec3* sad_vec3_unit(sad_vec3* dst)
{
    sad_float length = sad_vec3_len(dst);
    return sad_vec3_div(dst, length);
}

sad_vec3* sad_vec3_lerp(sad_vec3* from_dst, sad_vec3* to, sad_float alpha)
{
    sad_vec3 step = *to;
    sad_vec3_mul(sad_vec3_sub(&step, from_dst), alpha);
    return sad_vec3_add(from_dst, &step);
}

sad_vec3* sad_vec3_rotate(sad_vec3* dst, sad_quat* rotation)
{
    sad_mat4 m;
    return sad_vec3_transform(dst, sad_mat4_rotation(&m, rotation));
}

sad_vec3* sad_vec3_transform(sad_vec3* dst, sad_mat4* t)
{
    sad_vec3 a = *dst;
    dst->x = a.x * t->m[0] + a.y * t->m[1] + a.z * t->m[2] + t->m[3];
    dst->y = a.x * t->m[4] + a.y * t->m[5] + a.z * t->m[6] + t->m[7];
    dst->z = a.x * t->m[8] + a.y * t->m[9] + a.z * t->m[10] + t->m[11];
    return dst;
}

sad_int sad_vec3_cmp(sad_vec3* a, sad_vec3* b)
{
    if (a->x != b->x) return a->x > b->x ? 1 : -1;
    if (a->y != b->y) return a->y > b->y ? 1 : -1;
    if (a->z != b->z) return a->z > b->z ? 1 : -1;
    return 0;
}

/*
* sad_vec4
* ----------------------------------------------------------------------------------------------- */

sad_vec4* sad_vec4_set(sad_vec4* dst, sad_float x, sad_float y, sad_float z, sad_float w)
{
    dst->x = x;
    dst->y = y;
    dst->z = z;
    dst->w = w;
    return dst;
}

sad_vec4* sad_vec4_add(sad_vec4* dst, sad_vec4* b)
{
    dst->x += b->x;
    dst->y += b->y;
    dst->z += b->z;
    dst->w += b->w;
    return dst;
}

sad_vec4* sad_vec4_sub(sad_vec4* dst, sad_vec4* b)
{
    dst->x -= b->x;
    dst->y -= b->y;
    dst->z -= b->z;
    dst->w -= b->w;
    return dst;
}

sad_vec4* sad_vec4_mul(sad_vec4* dst, sad_float v)
{
    dst->x *= v;
    dst->y *= v;
    dst->z *= v;
    dst->w *= v;
    return dst;
}

sad_vec4* sad_vec4_div(sad_vec4* dst, sad_float v)
{
    dst->x /= v;
    dst->y /= v;
    dst->z /= v;
    dst->w /= v;
    return dst;
}

sad_float sad_vec4_dot(sad_vec4* a, sad_vec4* b)
{
    return a->x * b->x + a->y * b->y + a->z * b->z + a->w * b->w;
}

sad_float sad_vec4_len(sad_vec4* src)
{
    return sad_fsqrt(sad_vec4_dot(src, src));
}

sad_vec4* sad_vec4_unit(sad_vec4* dst)
{
    sad_float length = sad_vec4_len(dst);
    return sad_vec4_div(dst, length);
}

sad_vec4* sad_vec4_lerp(sad_vec4* from_dst, sad_vec4* to, sad_float alpha)
{
    sad_vec4 step = *to;
    sad_vec4_mul(sad_vec4_sub(&step, from_dst), alpha);
    return sad_vec4_add(from_dst, &step);
}

sad_vec4* sad_vec4_transform(sad_vec4* dst, sad_mat4* t)
{
    sad_vec4 a = *dst;
    dst->x = a.x * t->m[0] + a.y * t->m[1] + a.z * t->m[2] + a.w * t->m[3];
    dst->y = a.x * t->m[4] + a.y * t->m[5] + a.z * t->m[6] + a.w * t->m[7];
    dst->z = a.x * t->m[8] + a.y * t->m[9] + a.z * t->m[10] + a.w * t->m[11];
    dst->w = a.x * t->m[12] + a.y * t->m[13] + a.z * t->m[14] + a.w * t->m[15];
    return dst;
}

sad_int sad_vec4_cmp(sad_vec4* a, sad_vec4* b)
{
    if (a->x != b->x) return a->x > b->x ? 1 : -1;
    if (a->y != b->y) return a->y > b->y ? 1 : -1;
    if (a->z != b->z) return a->z > b->z ? 1 : -1;
    if (a->w != b->w) return a->w > b->w ? 1 : -1;
    return 0;
}

/*
* sad_quat
* ----------------------------------------------------------------------------------------------- */

sad_quat* sad_eul2quat(sad_quat* dst, sad_vec3* angles)
{
    sad_float cx = sad_fcos(sad_fdtor(angles->x * 0.5f));
    sad_float cy = sad_fcos(sad_fdtor(angles->y * 0.5f));
    sad_float cz = sad_fcos(sad_fdtor(angles->z * 0.5f));
    
    sad_float sx = sad_fsin(sad_fdtor(angles->x * 0.5f));
    sad_float sy = sad_fsin(sad_fdtor(angles->y * 0.5f));
    sad_float sz = sad_fsin(sad_fdtor(angles->z * 0.5f));

    dst->x = cx * sy * sz - sx * cy * cz;
    dst->x = -cx * sy * cz - sx * cy * sz;
    dst->x = cx * cy * sz - sx * sy * cz;
    dst->x = cx * cy * cz + sx * sy * sz;
    return dst;
}

sad_quat* sad_ax2quat(sad_quat* dst, sad_vec3* axis, sad_float angle)
{
    sad_float cos = sad_fcos(sad_fdtor(angle * 0.5f));
    sad_float sin = sad_fsin(sad_fdtor(angle * 0.5f));
    sad_vec3 ax = *axis;

    sad_vec3_mul(&ax, sin);
    dst->x = ax.x;
    dst->y = ax.y;
    dst->z = ax.z;
    dst->w = cos;
    return dst;
}

sad_quat* sad_quat_add(sad_quat* dst, sad_quat* b)
{
    dst->x += b->x;
    dst->y += b->y;
    dst->z += b->z;
    dst->w += b->w;
    return dst;
}

sad_quat* sad_quat_sub(sad_quat* dst, sad_quat* b)
{
    dst->x -= b->x;
    dst->y -= b->y;
    dst->z -= b->z;
    dst->w -= b->w;
    return dst;
}

sad_quat* sad_quat_mul(sad_quat* dst, sad_float v)
{
    dst->x *= v;
    dst->y *= v;
    dst->z *= v;
    dst->w *= v;
    return dst;
}

sad_quat* sad_quat_div(sad_quat* dst, sad_float v)
{
    dst->x /= v;
    dst->y /= v;
    dst->z /= v;
    dst->w /= v;
    return dst;
}

sad_float sad_quat_dot(sad_quat* a, sad_quat* b)
{
    return a->x * b->x + a->y * b->y + a->z * b->z + a->w * b->w;
}

sad_float sad_quat_len(sad_quat* src)
{
    return sad_fsqrt(sad_quat_dot(src, src));
}

sad_quat* sad_quat_conj(sad_quat* dst)
{
    dst->x = -dst->x;
    dst->y = -dst->y;
    dst->z = -dst->z;
    return dst;
}

sad_quat* sad_quat_inv(sad_quat* dst)
{
    sad_float sqlen = sad_quat_dot(dst, dst);
    return sad_quat_div(sad_quat_conj(dst), sqlen);
}

sad_quat* sad_quat_unit(sad_quat* dst)
{
    sad_float length = sad_quat_len(dst);
    return sad_quat_div(dst, length);
}

sad_quat* sad_quat_lerp(sad_quat* from_dst, sad_quat* to, sad_float alpha)
{
    sad_quat step = *to;
    sad_quat_mul(sad_quat_sub(&step, from_dst), alpha);
    return sad_quat_add(from_dst, &step);
}

sad_quat* sad_quat_slerp(sad_quat* from_dst, sad_quat* to, sad_float alpha)
{
    sad_float dot = sad_quat_dot(from_dst, to);
    sad_float angle = sad_facos(dot);
    sad_quat tmp = *to;

    if (dot >= 0)
        sad_quat_mul(from_dst, (sad_float)-1.0);
    
    sad_quat_mul(from_dst, sad_fsin((sad_float)1.0 - alpha) * angle);
    sad_quat_mul(&tmp, sad_fsin(alpha * angle));
    sad_quat_add(from_dst, &tmp);
    return sad_quat_div(from_dst, sad_fsin(angle));
}

sad_quat* sad_quat_slerpl(sad_quat* from_dst, sad_quat* to, sad_float alpha)
{
    sad_float dot = sad_quat_dot(from_dst, to);
    sad_float angle = sad_facos(dot);
    sad_quat tmp = *to;

    if (dot < 0)
        sad_quat_mul(from_dst, (sad_float)-1.0);
    
    sad_quat_mul(from_dst, sad_fsin((sad_float)1.0 - alpha) * angle);
    sad_quat_mul(&tmp, sad_fsin(alpha * angle));
    sad_quat_add(from_dst, &tmp);
    return sad_quat_div(from_dst, sad_fsin(angle));
}

sad_quat* sad_quat_nlerp(sad_quat* from_dst, sad_quat* to, sad_float alpha)
{
    return sad_quat_unit(sad_quat_lerp(from_dst, to, alpha));
}

sad_quat* sad_quat_rotate(sad_quat* dst, sad_quat* b)
{
    sad_quat a = *dst;
    dst->x = (a.x * b->w) + (a.w * b->x) + (a.y * b->z) - (a.z * b->y);
    dst->y = (a.y * b->w) + (a.w * b->y) + (a.z * b->x) - (a.x * b->z);
    dst->z = (a.z * b->w) + (a.w * b->z) + (a.x * b->y) - (a.y * b->x);
    dst->w = (a.w * b->w) - (a.x * b->x) - (a.y * b->y) - (a.z * b->z);
    return dst;
}

/*
* sad_mat3
* ----------------------------------------------------------------------------------------------- */

sad_mat3* sad_mat3_rotation(sad_mat3* dst, sad_quat* r)
{
    dst->m[0] = (sad_float)1.0 - ((sad_float)2.0 * r->y * r->y) - ((sad_float)2.0 * r->z * r->z);
    dst->m[1] = ((sad_float)2.0 * r->x * r->y) - ((sad_float)2.0 * r->w * r->z);
    dst->m[2] = ((sad_float)2.0 * r->x * r->z) + ((sad_float)2.0 * r->w * r->y);
    
    dst->m[3] = ((sad_float)2.0 * r->x * r->y) + ((sad_float)2.0 * r->w * r->z);
    dst->m[4] = (sad_float)1.0 - ((sad_float)2.0 * r->x * r->x) - ((sad_float)2.0 * r->z * r->z);
    dst->m[5] = ((sad_float)2.0 * r->y * r->z) - ((sad_float)2.0 * r->w * r->x);
    
    dst->m[6] = ((sad_float)2.0 * r->x * r->z) - ((sad_float)2.0 * r->w * r->y);
    dst->m[7] = ((sad_float)2.0 * r->y * r->z) + ((sad_float)2.0 * r->w * r->x);
    dst->m[8] = (sad_float)1.0 - ((sad_float)2.0 * r->x * r->x) - ((sad_float)2.0 * r->y * r->y);
    return dst;
}

sad_mat3* sad_mat3_scale(sad_mat3* dst, sad_vec3* scale)
{
    dst->m[0] = scale->x;
    dst->m[1] = 0.0f;
    dst->m[2] = 0.0f;

    dst->m[3] = 0.0f;
    dst->m[4] = scale->y;
    dst->m[5] = 0.0f;

    dst->m[6] = 0.0f;
    dst->m[7] = 0.0f;
    dst->m[8] = scale->z;
    return dst;
}

sad_mat3* sad_mat3_from_mat4(sad_mat3* dst, sad_mat4* m)
{
    dst->m[0] = m->m[0];
    dst->m[1] = m->m[1];
    dst->m[2] = m->m[2];

    dst->m[3] = m->m[4];
    dst->m[4] = m->m[5];
    dst->m[5] = m->m[6];

    dst->m[6] = m->m[8];
    dst->m[7] = m->m[9];
    dst->m[8] = m->m[10];
    return dst;
}

sad_mat3* sad_mat3_add(sad_mat3* dst, sad_mat3* b)
{
    dst->m[0] += b->m[0];
    dst->m[1] += b->m[1];
    dst->m[2] += b->m[2];
    dst->m[3] += b->m[3];
    dst->m[4] += b->m[4];
    dst->m[5] += b->m[5];
    dst->m[6] += b->m[6];
    dst->m[7] += b->m[7];
    dst->m[8] += b->m[8];
    return dst;
}

sad_mat3* sad_mat3_sub(sad_mat3* dst, sad_mat3* b)
{
    dst->m[0] -= b->m[0];
    dst->m[1] -= b->m[1];
    dst->m[2] -= b->m[2];
    dst->m[3] -= b->m[3];
    dst->m[4] -= b->m[4];
    dst->m[5] -= b->m[5];
    dst->m[6] -= b->m[6];
    dst->m[7] -= b->m[7];
    dst->m[8] -= b->m[8];
    return dst;
}

sad_mat3* sad_mat3_mul(sad_mat3* dst, sad_mat3* b)
{
    sad_mat3 a = *dst;
    dst->m[0] = (a.m[0] * b->m[0]) + (a.m[1] * b->m[3]) + (a.m[2] * b->m[6]);
    dst->m[1] = (a.m[0] * b->m[1]) + (a.m[1] * b->m[4]) + (a.m[2] * b->m[7]);
    dst->m[2] = (a.m[0] * b->m[2]) + (a.m[1] * b->m[5]) + (a.m[2] * b->m[8]);
    dst->m[3] = (a.m[3] * b->m[0]) + (a.m[4] * b->m[3]) + (a.m[5] * b->m[6]);
    dst->m[4] = (a.m[3] * b->m[1]) + (a.m[4] * b->m[4]) + (a.m[5] * b->m[7]);
    dst->m[5] = (a.m[3] * b->m[2]) + (a.m[4] * b->m[5]) + (a.m[5] * b->m[8]);
    dst->m[6] = (a.m[6] * b->m[0]) + (a.m[7] * b->m[3]) + (a.m[8] * b->m[6]);
    dst->m[7] = (a.m[6] * b->m[1]) + (a.m[7] * b->m[4]) + (a.m[8] * b->m[7]);
    dst->m[8] = (a.m[6] * b->m[2]) + (a.m[7] * b->m[5]) + (a.m[8] * b->m[8]);
    return dst;
}

sad_mat3* sad_mat3_transpose(sad_mat3* dst)
{
    sad_mat3 a = *dst;
    dst->m[0] = a.m[0];
    dst->m[1] = a.m[3];
    dst->m[2] = a.m[6];
    dst->m[3] = a.m[1];
    dst->m[4] = a.m[4];
    dst->m[5] = a.m[7];
    dst->m[6] = a.m[2];
    dst->m[7] = a.m[5];
    dst->m[8] = a.m[8];
    return dst;
}


/*
* sad_mat4
* ----------------------------------------------------------------------------------------------- */

sad_mat4* sad_mat4_translation(sad_mat4* dst, sad_vec3* translation)
{
    const sad_mat4 m = {{
        1.0f, 0.0f, 0.0f, translation->x,
        0.0f, 1.0f, 0.0f, translation->y,
        0.0f, 0.0f, 1.0f, translation->z,
        0.0f, 0.0f, 0.0f, 1.0f
    }};

    memcpy(dst, &m, sizeof(sad_mat4));
    return dst;
}

sad_mat4* sad_mat4_rotation(sad_mat4* dst, sad_quat* r)
{
    dst->m[0] = (sad_float)1.0 - ((sad_float)2.0 * r->y * r->y) - ((sad_float)2.0 * r->z * r->z);
    dst->m[1] = ((sad_float)2.0 * r->x * r->y) - ((sad_float)2.0 * r->w * r->z);
    dst->m[2] = ((sad_float)2.0 * r->x * r->z) + ((sad_float)2.0 * r->w * r->y);
    dst->m[3] = (sad_float)0.0;

    dst->m[4] = ((sad_float)2.0 * r->x * r->y) + ((sad_float)2.0 * r->w * r->z);
    dst->m[5] = (sad_float)1.0 - ((sad_float)2.0 * r->x * r->x) - ((sad_float)2.0 * r->z * r->z);
    dst->m[6] = ((sad_float)2.0 * r->y * r->z) - ((sad_float)2.0 * r->w * r->x);
    dst->m[7] = (sad_float)0.0;

    dst->m[8] = ((sad_float)2.0 * r->x * r->z) - ((sad_float)2.0 * r->w * r->y);
    dst->m[9] = ((sad_float)2.0 * r->y * r->z) + ((sad_float)2.0 * r->w * r->x);
    dst->m[10] = (sad_float)1.0 - ((sad_float)2.0 * r->x * r->x) - ((sad_float)2.0 * r->y * r->y);
    dst->m[11] = (sad_float)0.0;

    dst->m[12] = (sad_float)0.0;
    dst->m[13] = (sad_float)0.0;
    dst->m[14] = (sad_float)0.0;
    dst->m[15] = (sad_float)1.0;
    return dst;
}

sad_mat4* sad_mat4_scale(sad_mat4* dst, sad_vec3* s)
{
    const sad_mat4 m = {{
        s->x, 0.0f, 0.0f, 0.0f,
        0.0f, s->y, 0.0f, 0.0f,
        0.0f, 0.0f, s->z, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    }};

    memcpy(dst, &m, sizeof(sad_mat4));
    return dst;
}

sad_mat4* sad_mat4_model(sad_mat4* dst, sad_t3d* model)
{
    sad_mat4 m;
    sad_mat4_translation(dst, &model->location);
    sad_mat4_rotation(&m, &model->rotation);
    sad_mat4_mul(dst, &m);
    sad_mat4_scale(&m, &model->scale);
    sad_mat4_mul(dst, &m);
    return dst;
}

sad_mat4* sad_mat4_view(sad_mat4* dst, sad_vec3* location, sad_vec3* rotation)
{
    sad_vec3 l = *location;
    sad_quat r;
    sad_mat4 t;
    
    return sad_mat4_mul(
        sad_mat4_rotation(dst, sad_quat_inv(sad_eul2quat(&r, rotation))),
        sad_mat4_translation(&t, sad_vec3_mul(&l, (sad_float)-1)));
}

sad_mat4* sad_mat4_mvp(sad_mat4* dst, sad_mat4* model, sad_mat4* view, sad_mat4* projection)
{
    sad_mat4 vm = *view;
    *dst = *projection;
    return sad_mat4_mul(dst, sad_mat4_mul(&vm, model));
}

sad_mat4* sad_mat4_from_mat3(sad_mat4* dst, sad_mat3* m)
{
    sad_mat4 tmp = {{
        m->m[0], m->m[1], m->m[2], (sad_float)0.0,
        m->m[3], m->m[4], m->m[5], (sad_float)0.0,
        m->m[6], m->m[7], m->m[8], (sad_float)0.0,
        (sad_float)0.0, (sad_float)0.0, (sad_float)0.0, (sad_float)1.0
    }};

    memcpy(dst, &tmp, sizeof(sad_mat4));
    return dst;
}

sad_mat4* sad_mat4_add(sad_mat4* dst, sad_mat4* b)
{
    dst->m[0] += b->m[0];
    dst->m[1] += b->m[1];
    dst->m[2] += b->m[2];
    dst->m[3] += b->m[3];
    dst->m[4] += b->m[4];
    dst->m[5] += b->m[5];
    dst->m[6] += b->m[6];
    dst->m[7] += b->m[7];
    dst->m[8] += b->m[8];
    dst->m[9] += b->m[9];
    dst->m[10] += b->m[10];
    dst->m[11] += b->m[11];
    dst->m[12] += b->m[12];
    dst->m[13] += b->m[13];
    dst->m[14] += b->m[14];
    dst->m[15] += b->m[15];
    return dst;
}

sad_mat4* sad_mat4_sub(sad_mat4* dst, sad_mat4* b)
{
    dst->m[0] -= b->m[0];
    dst->m[1] -= b->m[1];
    dst->m[2] -= b->m[2];
    dst->m[3] -= b->m[3];
    dst->m[4] -= b->m[4];
    dst->m[5] -= b->m[5];
    dst->m[6] -= b->m[6];
    dst->m[7] -= b->m[7];
    dst->m[8] -= b->m[8];
    dst->m[9] -= b->m[9];
    dst->m[10] -= b->m[10];
    dst->m[11] -= b->m[11];
    dst->m[12] -= b->m[12];
    dst->m[13] -= b->m[13];
    dst->m[14] -= b->m[14];
    dst->m[15] -= b->m[15];
    return dst;
}

sad_mat4* sad_mat4_mul(sad_mat4* dst, sad_mat4* b)
{
    sad_mat4 a = *dst;
    dst->m[0] = a.m[0] * b->m[0] + a.m[1] * b->m[4] + a.m[2] * b->m[8] + a.m[3] * b->m[12];
    dst->m[1] = a.m[0] * b->m[1] + a.m[1] * b->m[5] + a.m[2] * b->m[9] + a.m[3] * b->m[13];
    dst->m[2] = a.m[0] * b->m[2] + a.m[1] * b->m[6] + a.m[2] * b->m[10] + a.m[3] * b->m[14];
    dst->m[3] = a.m[0] * b->m[3] + a.m[1] * b->m[7] + a.m[2] * b->m[11] + a.m[3] * b->m[15];
    dst->m[4] = a.m[4] * b->m[0] + a.m[5] * b->m[4] + a.m[6] * b->m[8] + a.m[7] * b->m[12];
    dst->m[5] = a.m[4] * b->m[1] + a.m[5] * b->m[5] + a.m[6] * b->m[9] + a.m[7] * b->m[13];
    dst->m[6] = a.m[4] * b->m[2] + a.m[5] * b->m[6] + a.m[6] * b->m[10] + a.m[7] * b->m[14];
    dst->m[7] = a.m[4] * b->m[3] + a.m[5] * b->m[7] + a.m[6] * b->m[11] + a.m[7] * b->m[15];
    dst->m[8] = a.m[8] * b->m[0] + a.m[9] * b->m[4] + a.m[10] * b->m[8] + a.m[11] * b->m[12];
    dst->m[9] = a.m[8] * b->m[1] + a.m[9] * b->m[5] + a.m[10] * b->m[9] + a.m[11] * b->m[13];
    dst->m[10] = a.m[8] * b->m[2] + a.m[9] * b->m[6] + a.m[10] * b->m[10] + a.m[11] * b->m[14];
    dst->m[11] = a.m[8] * b->m[3] + a.m[9] * b->m[7] + a.m[10] * b->m[11] + a.m[11] * b->m[15];
    dst->m[12] = a.m[12] * b->m[0] + a.m[13] * b->m[4] + a.m[14] * b->m[8] + a.m[15] * b->m[12];
    dst->m[13] = a.m[12] * b->m[1] + a.m[13] * b->m[5] + a.m[14] * b->m[9] + a.m[15] * b->m[13];
    dst->m[14] = a.m[12] * b->m[2] + a.m[13] * b->m[6] + a.m[14] * b->m[10] + a.m[15] * b->m[14];
    dst->m[15] = a.m[12] * b->m[3] + a.m[13] * b->m[7] + a.m[14] * b->m[11] + a.m[15] * b->m[15];
    return dst;
}

sad_mat4* sad_mat4_transpose(sad_mat4* dst)
{
    sad_mat4 a = *dst;
    dst->m[1] = a.m[4];
    dst->m[2] = a.m[8];
    dst->m[3] = a.m[12];

    dst->m[4] = a.m[1];
    dst->m[6] = a.m[9];
    dst->m[7] = a.m[13];
    
    dst->m[8] = a.m[2];
    dst->m[9] = a.m[6];
    dst->m[11] = a.m[14];

    dst->m[12] = a.m[3];
    dst->m[13] = a.m[7];
    dst->m[14] = a.m[11];
    return dst;
}

/* 'far' <= abs('near') to get infinite far-plane. 'near' < 0 to reverse z. */
sad_mat4* sad_mat4_perspective_vk(sad_mat4* dst, sad_float fov, sad_float ratio, sad_float near, sad_float far)
{
    sad_float a, b, c, d;
    fov = sad_ftan(sad_fdtor(fov / (sad_float)2.0));
    a = (sad_float)1.0 / (ratio * fov);
    b = (sad_float)1.0 / fov;

    if (far > sad_fabs(near))
    {
        if (near > (sad_float)0.0)
        {
            c = far / (far - near);
            d = (-near * far) / (far - near);
        }
        else
        {
            /* reverse Z */
            c = near / (far + near);
            d = (-near * far) / (far + near);
        }
    }
    else
    {
        /* infinite far plane */
        if (near > (sad_float)0.0)
        {
            c = (sad_float)1.0;
            d = -near;
        }
        else
        {
            /* reverse Z */
            c = (sad_float)0.0;
            d = -near;
        }
    }
    memset(dst, 0, sizeof(sad_mat4));
    dst->m[0] = a;
    dst->m[6] = -b;
    dst->m[9] = c;
    dst->m[11] = d;
    dst->m[13] = (sad_float)1.0;
    return dst;
}

/* 'far' <= abs('near') to get infinite far-plane. 'near' < 0 to reverse z. */
sad_mat4* sad_mat4_perspective_gl(sad_mat4* dst, sad_float fov, sad_float ratio, sad_float near, sad_float far)
{
    sad_float a, b, c, d;
    fov = sad_ftan(sad_fdtor(fov / (sad_float)2.0));
    a = (sad_float)1.0 / (ratio * fov);
    b = (sad_float)1.0 / fov;

    if (far > sad_fabs(near))
    {
        if (near > (sad_float)0.0)
        {
            c = (far + near) / (far - near);
            d = ((sad_float)-2.0 * near * far) / (far - near);
        }
        else
        {
            /* reverse Z */
            c = -(far - near) / (far + near);
            d = ((sad_float)-2.0 * near * far) / (far + near);
        }
    }
    else
    {
        /* infinite far plane */
        if (near > (sad_float)0.0)
        {
            c = (sad_float)1.0;
            d = (sad_float)-2.0 * near;
        }
        else
        {
            /* reverse Z */
            c = (sad_float)0.0;
            d = -near;
        }
    }
    memset(dst, 0, sizeof(sad_mat4));
    dst->m[0] = a;
    dst->m[6] = b;
    dst->m[9] = c;
    dst->m[11] = d;
    dst->m[13] = (sad_float)1.0;
    return dst;
}
