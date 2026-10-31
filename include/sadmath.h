/* ==============================================================================================
 * Sad Lib Math
 * ============================================================================================== */

#ifndef SADMATH_H
#define SADMATH_H

#include "sadcore.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
* Type definitions
* ----------------------------------------------------------------------------------------------- */

typedef struct sad_vec2
{
	sad_float x;
	sad_float y;
} sad_vec2;

typedef struct sad_vec3
{
	sad_float x;
	sad_float y;
	sad_float z;
} sad_vec3;

typedef struct sad_vec4
{
	sad_float x;
	sad_float y;
	sad_float z;
	sad_float w;
} sad_vec4, sad_quat;

typedef struct sad_mat3
{
	sad_float m[9];
} sad_mat3;

typedef struct sad_mat4
{
	sad_float m[16];
} sad_mat4;

typedef struct sad_t3d
{
	sad_vec3 location;
	sad_quat rotation;
	sad_vec3 scale;
} sad_t3d;

/*
* sad_int
* ----------------------------------------------------------------------------------------------- */

SAD_API sad_int sad_iabs(sad_int v);
SAD_API sad_int sad_imin(sad_int a, sad_int b);
SAD_API sad_int sad_imax(sad_int a, sad_int b);
SAD_API sad_int sad_iclamp(sad_int v, sad_int min, sad_int max);
SAD_API sad_int sad_ipow(sad_int v, sad_int exp);

SAD_API sad_uint sad_umin(sad_uint a, sad_uint b);
SAD_API sad_uint sad_umax(sad_uint a, sad_uint b);
SAD_API sad_uint sad_uclamp(sad_uint v, sad_uint min, sad_uint max);
SAD_API sad_uint sad_upow(sad_uint v, sad_uint exp);

/*
* sad_float
* ----------------------------------------------------------------------------------------------- */

#define PI (sad_float)3.141592653589793
#define sad_frtod(r) (r * ((sad_float)180.0 / PI))
#define sad_fdtor(d) (d * (PI / (sad_float)180.0))

SAD_API sad_float sad_fabs(sad_float v);
SAD_API sad_float sad_fmin(sad_float a, sad_float b);
SAD_API sad_float sad_fmax(sad_float a, sad_float b);
SAD_API sad_float sad_ftrunc(sad_float v);
SAD_API sad_float sad_fround(sad_float v);
SAD_API sad_float sad_fmod(sad_float v, sad_float mod);
SAD_API sad_float sad_fclamp(sad_float v, sad_float min, sad_float max);
SAD_API sad_int sad_fisclose(sad_float a, sad_float b, sad_float epsilon);
SAD_API sad_float sad_fsin(sad_float v);
SAD_API sad_float sad_fasin(sad_float v);
SAD_API sad_float sad_fcos(sad_float v);
SAD_API sad_float sad_facos(sad_float v);
SAD_API sad_float sad_ftan(sad_float v);
SAD_API sad_float sad_fatan(sad_float v);
SAD_API sad_float sad_fsqrt(sad_float v);
SAD_API sad_float sad_fpow(sad_float v, sad_float exp);
SAD_API sad_float sad_flog2(sad_float v);
SAD_API sad_int sad_fcmp(sad_float a, sad_float b);

/*
* sad_vec2
* ----------------------------------------------------------------------------------------------- */

SAD_API sad_vec2* sad_vec2_set(sad_vec2* dst, sad_float x, sad_float y);
SAD_API sad_vec2* sad_vec2_add(sad_vec2* dst, sad_vec2* b);
SAD_API sad_vec2* sad_vec2_sub(sad_vec2* dst, sad_vec2* b);
SAD_API sad_vec2* sad_vec2_mul(sad_vec2* dst, sad_float v);
SAD_API sad_vec2* sad_vec2_div(sad_vec2* dst, sad_float v);
SAD_API sad_float sad_vec2_dot(sad_vec2* a, sad_vec2* b);
SAD_API sad_float sad_vec2_len(sad_vec2* src);
SAD_API sad_vec2* sad_vec2_unit(sad_vec2* dst);
SAD_API sad_vec2* sad_vec2_lerp(sad_vec2* from_dst, sad_vec2* to, sad_float alpha);
/* SAD_API sad_vec2* sad_vec2_rotate(sad_vec2* dst, sad_float degrees); */
/* SAD_API sad_vec2* sad_vec2_transform(sad_vec2* dst, sad_mat3* transform); */
SAD_API sad_int sad_vec2_cmp(sad_vec2* a, sad_vec2* b);

/*
* sad_vec3
* ----------------------------------------------------------------------------------------------- */

SAD_API sad_vec3* sad_vec3_set(sad_vec3* dst, sad_float x, sad_float y, sad_float z);
SAD_API sad_vec3* sad_vec3_add(sad_vec3* dst, sad_vec3* b);
SAD_API sad_vec3* sad_vec3_sub(sad_vec3* dst, sad_vec3* b);
SAD_API sad_vec3* sad_vec3_mul(sad_vec3* dst, sad_float v);
SAD_API sad_vec3* sad_vec3_div(sad_vec3* dst, sad_float v);
SAD_API sad_float sad_vec3_dot(sad_vec3* a, sad_vec3* b);
SAD_API sad_vec3* sad_vec3_cross(sad_vec3* dst, sad_vec3* b);
SAD_API sad_float sad_vec3_len(sad_vec3* src);
SAD_API sad_vec3* sad_vec3_unit(sad_vec3* dst);
SAD_API sad_vec3* sad_vec3_lerp(sad_vec3* from_dst, sad_vec3* to, sad_float alpha);
SAD_API sad_vec3* sad_vec3_rotate(sad_vec3* dst, sad_quat* rotation);
SAD_API sad_vec3* sad_vec3_transform(sad_vec3* dst, sad_mat4* transform);
SAD_API sad_int sad_vec3_cmp(sad_vec3* a, sad_vec3* b);

/*
* sad_vec4
* ----------------------------------------------------------------------------------------------- */

SAD_API sad_vec4* sad_vec4_set(sad_vec4* dst, sad_float x, sad_float y, sad_float z, sad_float w);
SAD_API sad_vec4* sad_vec4_add(sad_vec4* dst, sad_vec4* b);
SAD_API sad_vec4* sad_vec4_sub(sad_vec4* dst, sad_vec4* b);
SAD_API sad_vec4* sad_vec4_mul(sad_vec4* dst, sad_float v);
SAD_API sad_vec4* sad_vec4_div(sad_vec4* dst, sad_float v);
SAD_API sad_float sad_vec4_dot(sad_vec4* a, sad_vec4* b);
SAD_API sad_float sad_vec4_len(sad_vec4* src);
SAD_API sad_vec4* sad_vec4_unit(sad_vec4* dst);
SAD_API sad_vec4* sad_vec4_lerp(sad_vec4* from_dst, sad_vec4* to, sad_float alpha);
SAD_API sad_vec4* sad_vec4_transform(sad_vec4* dst, sad_mat4* src);
SAD_API sad_int sad_vec4_cmp(sad_vec4* a, sad_vec4* b);

/*
* sad_quat
* ----------------------------------------------------------------------------------------------- */

SAD_API sad_quat* sad_eul2quat(sad_quat* dst, sad_vec3* angles);
SAD_API sad_quat* sad_ax2quat(sad_quat* dst, sad_vec3* axis, sad_float angle);
SAD_API sad_quat* sad_quat_add(sad_quat* dst, sad_quat* b);
SAD_API sad_quat* sad_quat_sub(sad_quat* dst, sad_quat* b);
SAD_API sad_quat* sad_quat_mul(sad_quat* dst, sad_float v);
SAD_API sad_quat* sad_quat_div(sad_quat* dst, sad_float v);
SAD_API sad_float sad_quat_dot(sad_quat* a, sad_quat* b);
SAD_API sad_float sad_quat_len(sad_quat* src);
SAD_API sad_quat* sad_quat_conj(sad_quat* dst);
SAD_API sad_quat* sad_quat_inv(sad_quat* dst);
SAD_API sad_quat* sad_quat_unit(sad_quat* dst);
SAD_API sad_quat* sad_quat_lerp(sad_quat* from_dst, sad_quat* to, sad_float alpha);
SAD_API sad_quat* sad_quat_slerp(sad_quat* from_dst, sad_quat* to, sad_float alpha);
SAD_API sad_quat* sad_quat_slerpl(sad_quat* from_dst, sad_quat* to, sad_float alpha);
SAD_API sad_quat* sad_quat_nlerp(sad_quat* from_dst, sad_quat* to, sad_float alpha);
SAD_API sad_quat* sad_quat_rotate(sad_quat* dst, sad_quat* rotation);

/*
* sad_mat3
* ----------------------------------------------------------------------------------------------- */

SAD_API sad_mat3* sad_mat3_rotation(sad_mat3* dst, sad_quat* rotation);
SAD_API sad_mat3* sad_mat3_scale(sad_mat3* dst, sad_vec3* scale);
SAD_API sad_mat3* sad_mat3_from_mat4(sad_mat3* dst, sad_mat4* m);
SAD_API sad_mat3* sad_mat3_add(sad_mat3* dst, sad_mat3* b);
SAD_API sad_mat3* sad_mat3_sub(sad_mat3* dst, sad_mat3* b);
SAD_API sad_mat3* sad_mat3_mul(sad_mat3* dst, sad_mat3* b);
SAD_API sad_mat3* sad_mat3_transpose(sad_mat3* dst);

/*
* sad_mat4
* ----------------------------------------------------------------------------------------------- */

SAD_API sad_mat4* sad_mat4_translation(sad_mat4* dst, sad_vec3* translation);
SAD_API sad_mat4* sad_mat4_rotation(sad_mat4* dst, sad_quat* rotation);
SAD_API sad_mat4* sad_mat4_scale(sad_mat4* dst, sad_vec3* scale);
SAD_API sad_mat4* sad_mat4_model(sad_mat4* dst, sad_t3d* model);
SAD_API sad_mat4* sad_mat4_view(sad_mat4* dst, sad_vec3* location, sad_vec3* rotation);
SAD_API sad_mat4* sad_mat4_mvp(sad_mat4* dst, sad_mat4* model, sad_mat4* view, sad_mat4* projection);
SAD_API sad_mat4* sad_mat4_from_mat3(sad_mat4* dst, sad_mat3* m);
SAD_API sad_mat4* sad_mat4_add(sad_mat4* dst, sad_mat4* b);
SAD_API sad_mat4* sad_mat4_sub(sad_mat4* dst, sad_mat4* b);
SAD_API sad_mat4* sad_mat4_mul(sad_mat4* dst, sad_mat4* b);
SAD_API sad_mat4* sad_mat4_transpose(sad_mat4* dst);
/* 'far' <= abs('near') to get infinite far-plane. 'near' < 0 to reverse z. */
SAD_API sad_mat4* sad_mat4_perspective_vk(sad_mat4* dst, sad_float fov, sad_float ratio, sad_float near, sad_float far);
/* 'far' <= abs('near') to get infinite far-plane. 'near' < 0 to reverse z. */
SAD_API sad_mat4* sad_mat4_perspective_gl(sad_mat4* dst, sad_float fov, sad_float ratio, sad_float near, sad_float far);

#ifdef __cplusplus
}
#endif

#endif /* SADMATH_H */
