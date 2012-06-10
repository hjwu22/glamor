/*
 * Copyright © 2009 Intel Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *
 * Authors:
 *    Zhigang Gong <zhigang.gong@linux.intel.com>
 *
 */

#ifndef GLAMOR_PRIV_H
#error This file can only be included by glamor_priv.h
#endif

#ifndef __GLAMOR_UTILS_H__
#define __GLAMOR_UTILS_H__

#define v_from_x_coord_x(_xscale_, _x_)          ( 2 * (_x_) * (_xscale_) - 1.0)
#define v_from_x_coord_y(_yscale_, _y_)          (-2 * (_y_) * (_yscale_) + 1.0)
#define v_from_x_coord_y_inverted(_yscale_, _y_) (2 * (_y_) * (_yscale_) - 1.0)
#define t_from_x_coord_x(_xscale_, _x_)          ((_x_) * (_xscale_))
#define t_from_x_coord_y(_yscale_, _y_)          (1.0 - (_y_) * (_yscale_))
#define t_from_x_coord_y_inverted(_yscale_, _y_) ((_y_) * (_yscale_))

#define pixmap_priv_get_dest_scale(_pixmap_priv_, _pxscale_, _pyscale_)	\
   do {									\
    *(_pxscale_) = 1.0 / (_pixmap_priv_)->base.pixmap->drawable.width;			\
    *(_pyscale_) = 1.0 / (_pixmap_priv_)->base.pixmap->drawable.height;			\
  } while(0)

#define pixmap_priv_get_scale(_pixmap_priv_, _pxscale_, _pyscale_)	\
   do {									\
    *(_pxscale_) = 1.0 / (_pixmap_priv_)->base.fbo->width;			\
    *(_pyscale_) = 1.0 / (_pixmap_priv_)->base.fbo->height;			\
  } while(0)

#define GLAMOR_PIXMAP_FBO_NOT_EAXCT_SIZE(priv)			\
   (priv->base.fbo->width != priv->base.pixmap->drawable.width 	\
      || priv->base.fbo->height != priv->base.pixmap->drawable.height)	\

#define PIXMAP_PRIV_GET_ACTUAL_SIZE(priv, w, h)			\
  do {								\
	if (priv->type == GLAMOR_TEXTURE_LARGE) {		\
		w = priv->large.box.x2 - priv->large.box.x1;	\
		h = priv->large.box.y2 - priv->large.box.y1;	\
	} else {						\
		w = priv->base.pixmap->drawable.width;		\
		h = priv->base.pixmap->drawable.height;		\
	}							\
  } while(0)

#define glamor_pixmap_fbo_fix_wh_ratio(wh, priv)  		\
  do {								\
	int actual_w, actual_h;					\
	PIXMAP_PRIV_GET_ACTUAL_SIZE(priv, actual_w, actual_h);	\
	wh[0] = (float)priv->base.fbo->width / actual_w;	\
	wh[1] = (float)priv->base.fbo->height / actual_h;	\
  } while(0)

#define pixmap_priv_get_fbo_off(_priv_, _xoff_, _yoff_)		\
   do {								\
	if (_priv_ && (_priv_)->type				\
		== GLAMOR_TEXTURE_LARGE) {			\
		*(_xoff_) = - (_priv_)->large.box.x1;	\
		*(_yoff_) = - (_priv_)->large.box.y1;	\
	} else {						\
		*(_xoff_) = 0;					\
		*(_yoff_) = 0;					\
	}							\
   } while(0)

#define xFixedToFloat(_val_) ((float)xFixedToInt(_val_)			\
			      + ((float)xFixedFrac(_val_) / 65536.0))

#define glamor_picture_get_matrixf(_picture_, _matrix_)			\
  do {									\
    int _i_;								\
    if ((_picture_)->transform)						\
      {									\
	for(_i_ = 0; _i_ < 3; _i_++)					\
	  {								\
	    (_matrix_)[_i_ * 3 + 0] =					\
	      xFixedToFloat((_picture_)->transform->matrix[_i_][0]);	\
	    (_matrix_)[_i_ * 3 + 1] =					\
	      xFixedToFloat((_picture_)->transform->matrix[_i_][1]);	\
	    (_matrix_)[_i_ * 3 + 2] = \
	      xFixedToFloat((_picture_)->transform->matrix[_i_][2]);	\
	  }								\
      }									\
  }  while(0)

#define glamor_transform_point(matrix, tx, ty, x, y)			\
  do {									\
    int i;								\
    float result[4];							\
    for (i = 0; i < 3; i++) {						\
      result[i] = (matrix)[i * 3] * (x) + (matrix)[i * 3 + 1] * (y)	\
	+ (matrix)[i * 3 + 2];						\
    }									\
    tx = result[0] / result[2];						\
    ty = result[1] / result[2];						\
  } while(0)

#define _glamor_set_normalize_tpoint(xscale, yscale, _tx_, _ty_,	\
				     texcoord, yInverted)		\
  do {									\
	(texcoord)[0] = t_from_x_coord_x(xscale, _tx_);			\
	if (yInverted)							\
		(texcoord)[1] = t_from_x_coord_y_inverted(yscale, _ty_);\
	else								\
		(texcoord)[1] = t_from_x_coord_y(yscale, _ty_);		\
        DEBUGF("normalized point tx %f ty %f \n", (texcoord)[0],	\
		(texcoord)[1]);						\
  } while(0)

#define glamor_set_transformed_point(priv, matrix, xscale,		\
				     yscale, texcoord,			\
                                     x, y, 				\
				     yInverted)				\
  do {									\
    float tx, ty;							\
    int fbo_x_off, fbo_y_off;						\
    pixmap_priv_get_fbo_off(priv, &fbo_x_off, &fbo_y_off);		\
    glamor_transform_point(matrix, tx, ty, x, y);			\
    DEBUGF("tx %f ty %f fbooff %d %d \n",				\
	    tx, ty, fbo_x_off, fbo_y_off);				\
									\
    tx += fbo_x_off;							\
    ty += fbo_y_off;							\
    (texcoord)[0] = t_from_x_coord_x(xscale, tx);			\
    if (yInverted)							\
      (texcoord)[1] = t_from_x_coord_y_inverted(yscale, ty);		\
    else								\
      (texcoord)[1] = t_from_x_coord_y(yscale, ty);			\
    DEBUGF("normalized tx %f ty %f \n", (texcoord)[0], (texcoord)[1]);	\
  } while(0)

#define glamor_set_transformed_normalize_tcoords( priv,			\
						  matrix,		\
						  xscale,		\
						  yscale,		\
                                                  tx1, ty1, tx2, ty2,   \
                                                  yInverted, texcoords)	\
  do {									\
    glamor_set_transformed_point(priv, matrix, xscale, yscale,		\
				 texcoords, tx1, ty1,			\
				 yInverted);				\
    glamor_set_transformed_point(priv, matrix, xscale, yscale,		\
				 texcoords + 2, tx2, ty1,		\
				 yInverted);				\
    glamor_set_transformed_point(priv, matrix, xscale, yscale,		\
				 texcoords + 4, tx2, ty2,		\
				 yInverted);				\
    glamor_set_transformed_point(priv, matrix, xscale, yscale,		\
				 texcoords + 6, tx1, ty2,		\
				 yInverted);				\
  } while (0)

#define _glamor_set_normalize_tcoords(xscale, yscale, tx1,		\
				      ty1, tx2, ty2,			\
				      yInverted, vertices)		\
  do {									\
    (vertices)[0] = t_from_x_coord_x(xscale, tx1);			\
    (vertices)[2] = t_from_x_coord_x(xscale, tx2);			\
    (vertices)[4] = (vertices)[2];					\
    (vertices)[6] = (vertices)[0];					\
    if (yInverted) {							\
      (vertices)[1] = t_from_x_coord_y_inverted(yscale, ty1);		\
      (vertices)[5] = t_from_x_coord_y_inverted(yscale, ty2);		\
    }									\
    else {								\
      (vertices)[1] = t_from_x_coord_y(yscale, ty1);			\
      (vertices)[5] = t_from_x_coord_y(yscale, ty2);			\
    }									\
    (vertices)[3] = (vertices)[1];					\
    (vertices)[7] = (vertices)[5];					\
    DEBUGF("texture %f %f %f %f\n", tx1, ty1, tx2, ty2);		\
    DEBUGF("texture %f %f %f %f\n", (vertices)[0], (vertices)[1],	\
	(vertices)[2], (vertices)[3]);					\
    DEBUGF("texture %f %f %f %f\n", (vertices)[4], (vertices)[5],	\
	(vertices)[6], (vertices)[7]);					\
  } while(0)

#define glamor_set_normalize_tcoords(priv, xscale, yscale,		\
				     x1, y1, x2, y2,			\
                                     yInverted, vertices)		\
  do {									\
     float tx1, tx2, ty1, ty2;						\
     int fbo_x_off, fbo_y_off;						\
     pixmap_priv_get_fbo_off(priv, &fbo_x_off, &fbo_y_off);		\
     tx1 = x1 + fbo_x_off; 						\
     tx2 = x2 + fbo_x_off;						\
     ty1 = y1 + fbo_y_off;						\
     ty2 = y2 + fbo_y_off;						\
     _glamor_set_normalize_tcoords(xscale, yscale, tx1, ty1,		\
				   tx2, ty2, yInverted, vertices);	\
 } while(0)

#define glamor_set_tcoords(width, height, x1, y1, x2, y2,	\
			   yInverted, vertices)			\
    do {							\
	(vertices)[0] = (x1);					\
	(vertices)[2] = (x2);					\
	(vertices)[4] = (vertices)[2];				\
	(vertices)[6] = (vertices)[0];				\
	if (yInverted) {					\
	    (vertices)[1] = (y1);				\
	    (vertices)[5] = (y2);				\
	}							\
	else {							\
	    (vertices)[1] = height - (y2);			\
	    (vertices)[5] = height - (y1);			\
	}							\
	(vertices)[3] = (vertices)[1];				\
	(vertices)[7] = (vertices)[5];				\
    } while(0)


#define glamor_set_normalize_vcoords(priv, xscale, yscale,		\
				     x1, y1, x2, y2,			\
                                     yInverted, vertices)		\
  do {									\
    int fbo_x_off, fbo_y_off;						\
    pixmap_priv_get_fbo_off(priv, &fbo_x_off, &fbo_y_off);		\
    (vertices)[0] = v_from_x_coord_x(xscale, x1 + fbo_x_off);		\
    (vertices)[2] = v_from_x_coord_x(xscale, x2 + fbo_x_off);		\
    (vertices)[4] = (vertices)[2];					\
    (vertices)[6] = (vertices)[0];					\
    if (yInverted) {							\
      (vertices)[1] = v_from_x_coord_y_inverted(yscale, y1 + fbo_y_off);\
      (vertices)[5] = v_from_x_coord_y_inverted(yscale, y2 + fbo_y_off);\
    }									\
    else {								\
      (vertices)[1] = v_from_x_coord_y(yscale, y1 + fbo_y_off);		\
      (vertices)[5] = v_from_x_coord_y(yscale, y2 + fbo_y_off);		\
    }									\
    (vertices)[3] = (vertices)[1];					\
    (vertices)[7] = (vertices)[5];					\
  } while(0)

#define glamor_set_normalize_pt(xscale, yscale, x, y,		\
                                yInverted, pt)			\
    do {							\
        (pt)[0] = t_from_x_coord_x(xscale, x);			\
        if (yInverted) {					\
            (pt)[1] = t_from_x_coord_y_inverted(yscale, y);	\
        } else {						\
            (pt)[1] = t_from_x_coord_y(yscale, y);		\
        }							\
    } while(0)

#define glamor_set_circle_centre(width, height, x, y,	\
				 yInverted, c)		\
    do {						\
        (c)[0] = (float)x;				\
        if (yInverted) {				\
            (c)[1] = (float)y;				\
        } else {					\
            (c)[1] = (float)height - (float)y;		\
        }						\
    } while(0)

inline static void
glamor_calculate_boxes_bound(BoxPtr bound, BoxPtr boxes, int nbox)
{
	int x_min, y_min;
	int x_max, y_max;
	int i;
	x_min = y_min = MAXSHORT;
	x_max = y_max = MINSHORT;
	for (i = 0; i < nbox; i++) {
		if (x_min > boxes[i].x1)
			x_min = boxes[i].x1;
		if (y_min > boxes[i].y1)
			y_min = boxes[i].y1;

		if (x_max < boxes[i].x2)
			x_max = boxes[i].x2;
		if (y_max < boxes[i].y2)
			y_max = boxes[i].y2;
	}
	bound->x1 = x_min;
	bound->y1 = y_min;
	bound->x2 = x_max;
	bound->y2 = y_max;
}

inline static void
glamor_transform_boxes(BoxPtr boxes, int nbox, int dx, int dy)
{
	int i;
	for (i = 0; i < nbox; i++) {
		boxes[i].x1 += dx;
		boxes[i].y1 += dy;
		boxes[i].x2 += dx;
		boxes[i].y2 += dy;
	}
}

#define ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))
#define ALIGN(i,m)	(((i) + (m) - 1) & ~((m) - 1))
#define MIN(a,b)	((a) < (b) ? (a) : (b))
#define MAX(a,b)	((a) > (b) ? (a) : (b))

#define glamor_check_fbo_size(_glamor_,_w_, _h_)    ((_w_) > 0 && (_h_) > 0 \
                                                    && (_w_) <= _glamor_->max_fbo_size  \
                                                    && (_h_) <= _glamor_->max_fbo_size)

/* For 1bpp pixmap, we don't store it as texture. */
#define glamor_check_pixmap_fbo_depth(_depth_) (			\
						_depth_ == 8		\
						|| _depth_ == 15	\
						|| _depth_ == 16	\
						|| _depth_ == 24	\
						|| _depth_ == 30	\
						|| _depth_ == 32)

#define GLAMOR_PIXMAP_PRIV_IS_PICTURE(pixmap_priv) (pixmap_priv && pixmap_priv->base.is_picture == 1)
#define GLAMOR_PIXMAP_PRIV_HAS_FBO(pixmap_priv)    (pixmap_priv && pixmap_priv->base.gl_fbo == GLAMOR_FBO_NORMAL)
#define GLAMOR_PIXMAP_PRIV_HAS_FBO_DOWNLOADED(pixmap_priv)    (pixmap_priv && (pixmap_priv->base.gl_fbo == GLAMOR_FBO_DOWNLOADED))

/**
 * Borrow from uxa.
 */
static inline CARD32
format_for_depth(int depth)
{
	switch (depth) {
	case 1:
		return PICT_a1;
	case 4:
		return PICT_a4;
	case 8:
		return PICT_a8;
	case 15:
		return PICT_x1r5g5b5;
	case 16:
		return PICT_r5g6b5;
	default:
	case 24:
		return PICT_x8r8g8b8;
#if XORG_VERSION_CURRENT >= 10699900
	case 30:
		return PICT_x2r10g10b10;
#endif
	case 32:
		return PICT_a8r8g8b8;
	}
}

static inline void
gl_iformat_for_depth(int depth, GLenum * format)
{
	switch (depth) {
#ifndef GLAMOR_GLES2
	case 1:
	case 8:
		*format = GL_ALPHA;
		break;
#endif
	default:
		*format = GL_RGBA;
		break;
       }
}

static inline CARD32
format_for_pixmap(PixmapPtr pixmap)
{
	glamor_pixmap_private *pixmap_priv;
	PictFormatShort pict_format;

	pixmap_priv = glamor_get_pixmap_private(pixmap);
	if (GLAMOR_PIXMAP_PRIV_IS_PICTURE(pixmap_priv))
		pict_format = pixmap_priv->base.picture->format;
	else
		pict_format = format_for_depth(pixmap->drawable.depth);

	return pict_format;
}

#define REVERT_NONE       		0
#define REVERT_NORMAL     		1
#define REVERT_DOWNLOADING_A1		2
#define REVERT_UPLOADING_A1		3
#define REVERT_DOWNLOADING_2_10_10_10 	4
#define REVERT_UPLOADING_2_10_10_10 	5
#define REVERT_DOWNLOADING_1_5_5_5  	7
#define REVERT_UPLOADING_1_5_5_5    	8
#define REVERT_DOWNLOADING_10_10_10_2 	9
#define REVERT_UPLOADING_10_10_10_2 	10

#define SWAP_NONE_DOWNLOADING  	0
#define SWAP_DOWNLOADING  	1
#define SWAP_UPLOADING	  	2
#define SWAP_NONE_UPLOADING	3

/*
 * Map picture's format to the correct gl texture format and type.
 * no_alpha is used to indicate whehter we need to wire alpha to 1. 
 *
 * Although opengl support A1/GL_BITMAP, we still don't use it
 * here, it seems that mesa has bugs when uploading a A1 bitmap.
 *
 * Return 0 if find a matched texture type. Otherwise return -1.
 **/
#ifndef GLAMOR_GLES2
static inline int
glamor_get_tex_format_type_from_pictformat(PictFormatShort format,
					   GLenum * tex_format,
					   GLenum * tex_type,
					   int *no_alpha,
					   int *revert,
					   int *swap_rb,
					   int is_upload)

{
	*no_alpha = 0;
	*revert = REVERT_NONE;
	*swap_rb = is_upload ? SWAP_NONE_UPLOADING : SWAP_NONE_DOWNLOADING;
	switch (format) {
	case PICT_a1:
		*tex_format = GL_ALPHA;
		*tex_type = GL_UNSIGNED_BYTE;
		*revert = is_upload ? REVERT_UPLOADING_A1 : REVERT_DOWNLOADING_A1;
		break;
	case PICT_b8g8r8x8:
		*no_alpha = 1;
	case PICT_b8g8r8a8:
		*tex_format = GL_BGRA;
		*tex_type = GL_UNSIGNED_INT_8_8_8_8;
		break;

	case PICT_x8r8g8b8:
		*no_alpha = 1;
	case PICT_a8r8g8b8:
		*tex_format = GL_BGRA;
		*tex_type = GL_UNSIGNED_INT_8_8_8_8_REV;
		break;
	case PICT_x8b8g8r8:
		*no_alpha = 1;
	case PICT_a8b8g8r8:
		*tex_format = GL_RGBA;
		*tex_type = GL_UNSIGNED_INT_8_8_8_8_REV;
		break;
	case PICT_x2r10g10b10:
		*no_alpha = 1;
	case PICT_a2r10g10b10:
		*tex_format = GL_BGRA;
		*tex_type = GL_UNSIGNED_INT_2_10_10_10_REV;
		break;
	case PICT_x2b10g10r10:
		*no_alpha = 1;
	case PICT_a2b10g10r10:
		*tex_format = GL_RGBA;
		*tex_type = GL_UNSIGNED_INT_2_10_10_10_REV;
		break;

	case PICT_r5g6b5:
		*tex_format = GL_RGB;
		*tex_type = GL_UNSIGNED_SHORT_5_6_5;
		break;
	case PICT_b5g6r5:
		*tex_format = GL_RGB;
		*tex_type = GL_UNSIGNED_SHORT_5_6_5_REV;
		break;
	case PICT_x1b5g5r5:
		*no_alpha = 1;
	case PICT_a1b5g5r5:
		*tex_format = GL_RGBA;
		*tex_type = GL_UNSIGNED_SHORT_1_5_5_5_REV;
		break;

	case PICT_x1r5g5b5:
		*no_alpha = 1;
	case PICT_a1r5g5b5:
		*tex_format = GL_BGRA;
		*tex_type = GL_UNSIGNED_SHORT_1_5_5_5_REV;
		break;
	case PICT_a8:
		*tex_format = GL_ALPHA;
		*tex_type = GL_UNSIGNED_BYTE;
		break;
	case PICT_x4r4g4b4:
		*no_alpha = 1;
	case PICT_a4r4g4b4:
		*tex_format = GL_BGRA;
		*tex_type = GL_UNSIGNED_SHORT_4_4_4_4_REV;
		break;

	case PICT_x4b4g4r4:
		*no_alpha = 1;
	case PICT_a4b4g4r4:
		*tex_format = GL_RGBA;
		*tex_type = GL_UNSIGNED_SHORT_4_4_4_4_REV;
		break;

	default:
		LogMessageVerb(X_INFO, 0,
			       "fail to get matched format for %x \n",
			       format);
		return -1;
	}
	return 0;
}

/* Currently, we use RGBA to represent all formats. */
inline static int cache_format(GLenum format)
{
	switch (format) {
	case GL_ALPHA:
		return 1;
	case GL_RGBA:
		return 0;
	default:
		return -1;
	}
}

#else
#define IS_LITTLE_ENDIAN  (IMAGE_BYTE_ORDER == LSBFirst)

static inline int
glamor_get_tex_format_type_from_pictformat(PictFormatShort format,
					   GLenum * tex_format,
					   GLenum * tex_type,
					   int *no_alpha,
					   int *revert,
					   int *swap_rb,
					   int is_upload)
{
	int need_swap_rb = 0;

	*no_alpha = 0;
	*revert = IS_LITTLE_ENDIAN ? REVERT_NONE : REVERT_NORMAL;

	switch (format) {
	case PICT_b8g8r8x8:
		*no_alpha = 1;
	case PICT_b8g8r8a8:
		*tex_format = GL_RGBA;
		*tex_type = GL_UNSIGNED_BYTE;
		need_swap_rb = 1;
		*revert = IS_LITTLE_ENDIAN ? REVERT_NORMAL : REVERT_NONE;
		break;

	case PICT_x8r8g8b8:
		*no_alpha = 1;
	case PICT_a8r8g8b8:
		*tex_format = GL_RGBA;
		*tex_type = GL_UNSIGNED_BYTE;
		need_swap_rb = 1;
		break;

	case PICT_x8b8g8r8:
		*no_alpha = 1;
	case PICT_a8b8g8r8:
		*tex_format = GL_RGBA;
		*tex_type = GL_UNSIGNED_BYTE;
		break;

	case PICT_x2r10g10b10:
		*no_alpha = 1;
	case PICT_a2r10g10b10:
		*tex_format = GL_RGBA;
		/* glReadPixmap doesn't support GL_UNSIGNED_INT_10_10_10_2.
		 * we have to use GL_UNSIGNED_BYTE and do the conversion in
		 * shader latter.*/
		*tex_type = GL_UNSIGNED_BYTE;
		if (is_upload == 1) {
			if (!IS_LITTLE_ENDIAN)
				*revert = REVERT_UPLOADING_10_10_10_2;
			else
				*revert = REVERT_UPLOADING_2_10_10_10;
		}
		else {
			if (!IS_LITTLE_ENDIAN) {
				*revert = REVERT_DOWNLOADING_10_10_10_2;
			}
			else {
				*revert = REVERT_DOWNLOADING_2_10_10_10;
			}
		}
		need_swap_rb = 1;

		break;

	case PICT_x2b10g10r10:
		*no_alpha = 1;
	case PICT_a2b10g10r10:
		*tex_format = GL_RGBA;
		*tex_type = GL_UNSIGNED_BYTE;
		if (is_upload == 1) {
			if (!IS_LITTLE_ENDIAN)
				*revert = REVERT_UPLOADING_10_10_10_2;
			else
				*revert = REVERT_UPLOADING_2_10_10_10;
		}
		else {
			if (!IS_LITTLE_ENDIAN) {
				*revert = REVERT_DOWNLOADING_10_10_10_2;
			}
			else {
				*revert = REVERT_DOWNLOADING_2_10_10_10;
			}
		}
		break;

	case PICT_r5g6b5:
		*tex_format = GL_RGB;
		*tex_type = GL_UNSIGNED_SHORT_5_6_5;
		*revert = IS_LITTLE_ENDIAN ? REVERT_NONE : REVERT_NORMAL;

		break;

	case PICT_b5g6r5:
		*tex_format = GL_RGB;
		*tex_type = GL_UNSIGNED_SHORT_5_6_5;
		need_swap_rb = IS_LITTLE_ENDIAN ? 1 : 0;;
		break;

	case PICT_x1b5g5r5:
		*no_alpha = 1;
	case PICT_a1b5g5r5:
		*tex_format = GL_RGBA;
		*tex_type = GL_UNSIGNED_SHORT_5_5_5_1;
		if (IS_LITTLE_ENDIAN) {
			*revert = is_upload ? REVERT_UPLOADING_1_5_5_5 : REVERT_DOWNLOADING_1_5_5_5;
		} else
			*revert = REVERT_NONE;
		break;

	case PICT_x1r5g5b5:
		*no_alpha = 1;
	case PICT_a1r5g5b5:
		*tex_format = GL_RGBA;
		*tex_type = GL_UNSIGNED_SHORT_5_5_5_1;
		if (IS_LITTLE_ENDIAN) {
			*revert = is_upload ? REVERT_UPLOADING_1_5_5_5 : REVERT_DOWNLOADING_1_5_5_5;
		} else
			*revert = REVERT_NONE;
		need_swap_rb = 1;
		break;

	case PICT_a1:
		*tex_format = GL_ALPHA;
		*tex_type = GL_UNSIGNED_BYTE;
		*revert = is_upload ? REVERT_UPLOADING_A1 : REVERT_DOWNLOADING_A1;
		break;

	case PICT_a8:
		*tex_format = GL_ALPHA;
		*tex_type = GL_UNSIGNED_BYTE;
		*revert = REVERT_NONE;
		break;

	case PICT_x4r4g4b4:
		*no_alpha = 1;
	case PICT_a4r4g4b4:
		*tex_format = GL_RGBA;
		*tex_type = GL_UNSIGNED_SHORT_4_4_4_4;
		*revert = IS_LITTLE_ENDIAN ? REVERT_NORMAL : REVERT_NONE;
		need_swap_rb = 1;
		break;

	case PICT_x4b4g4r4:
		*no_alpha = 1;
	case PICT_a4b4g4r4:
		*tex_format = GL_RGBA;
		*tex_type = GL_UNSIGNED_SHORT_4_4_4_4;
		*revert = IS_LITTLE_ENDIAN ? REVERT_NORMAL : REVERT_NONE;
		break;

	default:
		LogMessageVerb(X_INFO, 0,
			       "fail to get matched format for %x \n",
			       format);
		return -1;
	}

	if (need_swap_rb)
		*swap_rb = is_upload ? SWAP_UPLOADING : SWAP_DOWNLOADING;
	else
		*swap_rb = is_upload ? SWAP_NONE_UPLOADING : SWAP_NONE_DOWNLOADING;
	return 0;
}

inline static int cache_format(GLenum format)
{
	switch (format) {
	case GL_ALPHA:
		return 2;
	case GL_RGB:
		return 1;
	case GL_RGBA:
		return 0;
	default:
		return -1;
	}
}

#endif


static inline int
glamor_get_tex_format_type_from_pixmap(PixmapPtr pixmap,
				       GLenum * format,
				       GLenum * type,
				       int *no_alpha,
				       int *revert,
				       int *swap_rb,
				       int is_upload)
{
	glamor_pixmap_private *pixmap_priv;
	PictFormatShort pict_format;

	pixmap_priv = glamor_get_pixmap_private(pixmap);
	if (GLAMOR_PIXMAP_PRIV_IS_PICTURE(pixmap_priv))
		pict_format = pixmap_priv->base.picture->format;
	else
		pict_format = format_for_depth(pixmap->drawable.depth);

	return glamor_get_tex_format_type_from_pictformat(pict_format,
							  format, type,
							  no_alpha,
							  revert,
							  swap_rb,
							  is_upload);
}


/* borrowed from uxa */
static inline Bool
glamor_get_rgba_from_pixel(CARD32 pixel,
			   float *red,
			   float *green,
			   float *blue, float *alpha, CARD32 format)
{
	int rbits, bbits, gbits, abits;
	int rshift, bshift, gshift, ashift;

	rbits = PICT_FORMAT_R(format);
	gbits = PICT_FORMAT_G(format);
	bbits = PICT_FORMAT_B(format);
	abits = PICT_FORMAT_A(format);

	if (PICT_FORMAT_TYPE(format) == PICT_TYPE_A) {
		rshift = gshift = bshift = ashift = 0;
	} else if (PICT_FORMAT_TYPE(format) == PICT_TYPE_ARGB) {
		bshift = 0;
		gshift = bbits;
		rshift = gshift + gbits;
		ashift = rshift + rbits;
	} else if (PICT_FORMAT_TYPE(format) == PICT_TYPE_ABGR) {
		rshift = 0;
		gshift = rbits;
		bshift = gshift + gbits;
		ashift = bshift + bbits;
#if XORG_VERSION_CURRENT >= 10699900
	} else if (PICT_FORMAT_TYPE(format) == PICT_TYPE_BGRA) {
		ashift = 0;
		rshift = abits;
		if (abits == 0)
			rshift = PICT_FORMAT_BPP(format) - (rbits + gbits +
							    bbits);
		gshift = rshift + rbits;
		bshift = gshift + gbits;
#endif
	} else {
		return FALSE;
	}
#define COLOR_INT_TO_FLOAT(_fc_, _p_, _s_, _bits_)	\
  *_fc_ = (((_p_) >> (_s_)) & (( 1 << (_bits_)) - 1))	\
    / (float)((1<<(_bits_)) - 1)

	if (rbits)
		COLOR_INT_TO_FLOAT(red, pixel, rshift, rbits);
	else
		*red = 0;

	if (gbits)
		COLOR_INT_TO_FLOAT(green, pixel, gshift, gbits);
	else
		*green = 0;

	if (bbits)
		COLOR_INT_TO_FLOAT(blue, pixel, bshift, bbits);
	else
		*blue = 0;

	if (abits)
		COLOR_INT_TO_FLOAT(alpha, pixel, ashift, abits);
	else
		*alpha = 1;

	return TRUE;
}

inline static Bool glamor_pict_format_is_compatible(PictFormatShort pict_format, int depth)
{
	GLenum iformat;

	gl_iformat_for_depth(depth, &iformat);
	switch (iformat) {
		case GL_RGBA:
			return (pict_format == PICT_a8r8g8b8 || pict_format == PICT_x8r8g8b8);
		case GL_ALPHA:
			return (pict_format == PICT_a8);
		default:
			return FALSE;
	}
}

/* return TRUE if we can access this pixmap at DDX driver. */
inline static Bool glamor_ddx_fallback_check_pixmap(DrawablePtr drawable)
{
	PixmapPtr pixmap = glamor_get_drawable_pixmap(drawable);
	glamor_pixmap_private *pixmap_priv = glamor_get_pixmap_private(pixmap);
	return (!pixmap_priv 
		|| (pixmap_priv->type == GLAMOR_TEXTURE_DRM
		    || pixmap_priv->type == GLAMOR_MEMORY
		    || pixmap_priv->type == GLAMOR_DRM_ONLY));
}

inline static Bool glamor_ddx_fallback_check_gc(GCPtr gc)
{
	PixmapPtr pixmap;
	if (!gc)
		return TRUE;
        switch (gc->fillStyle) {
        case FillStippled:
        case FillOpaqueStippled:
                pixmap = gc->stipple;
                break;
        case FillTiled:
                pixmap = gc->tile.pixmap;
                break;
	default:
		pixmap = NULL;
        }
	return (!pixmap || glamor_ddx_fallback_check_pixmap(&pixmap->drawable));
}
inline static Bool glamor_is_large_pixmap(PixmapPtr pixmap)
{
	glamor_pixmap_private *priv;

	priv = glamor_get_pixmap_private(pixmap);
	return (priv->type == GLAMOR_TEXTURE_LARGE);
}

inline static Bool glamor_is_large_picture(PicturePtr picture)
{
	PixmapPtr pixmap;

	if (picture->pDrawable) {
		pixmap = glamor_get_drawable_pixmap(picture->pDrawable);
		return glamor_is_large_pixmap(pixmap);
	}
	return FALSE;
}

inline static Bool glamor_tex_format_is_readable(GLenum format)
{
	return ((format == GL_RGBA || format == GL_RGB || format == GL_ALPHA));

}

static inline void _glamor_dump_pixmap_bits(PixmapPtr pixmap, int x, int y, int w, int h)
{
	int i,j;
	unsigned char * p = pixmap->devPrivate.ptr;
	int stride = pixmap->devKind;

	p = p + y * stride + x;

	for (i = 0; i < h; i++)
	{
		ErrorF("line %3d: ", i);
		for(j = 0; j < w; j++)
			ErrorF("%2d ", (p[j/8] & (1 << (j%8)))>>(j%8));
		p += stride;
		ErrorF("\n");
	}
}

static inline void _glamor_dump_pixmap_byte(PixmapPtr pixmap, int x, int y, int w, int h)
{
	int i,j;
	unsigned char * p = pixmap->devPrivate.ptr;
	int stride = pixmap->devKind;

	p = p + y * stride + x;

	for (i = 0; i < h; i++)
	{
		ErrorF("line %3d: ", i);
		for(j = 0; j < w; j++)
			ErrorF("%2x ", p[j]);
		p += stride;
		ErrorF("\n");
	}
}

static inline void _glamor_dump_pixmap_sword(PixmapPtr pixmap, int x, int y, int w, int h)
{
	int i,j;
	unsigned short * p = pixmap->devPrivate.ptr;
	int stride = pixmap->devKind / 2;

	p = p + y * stride + x;

	for (i = 0; i < h; i++)
	{
		ErrorF("line %3d: ", i);
		for(j = 0; j < w; j++)
			ErrorF("%2x ", p[j]);
		p += stride;
		ErrorF("\n");
	}
}

static inline void _glamor_dump_pixmap_word(PixmapPtr pixmap, int x, int y, int w, int h)
{
	int i,j;
	unsigned int * p = pixmap->devPrivate.ptr;
	int stride = pixmap->devKind / 4;

	p = p + y * stride + x;

	for (i = 0; i < h; i++)
	{
		ErrorF("line %3d: ", i);
		for(j = 0; j < w; j++)
			ErrorF("%2x ", p[j]);
		p += stride;
		ErrorF("\n");
	}
}

static inline void glamor_dump_pixmap(PixmapPtr pixmap, int x, int y, int w, int h)
{
	w = ((x + w) > pixmap->drawable.width) ? (pixmap->drawable.width - x) : w;
	h = ((y + h) > pixmap->drawable.height) ? (pixmap->drawable.height - y) : h;

	glamor_prepare_access(&pixmap->drawable, GLAMOR_ACCESS_RO);
	switch (pixmap->drawable.depth) {
	case 8:
		_glamor_dump_pixmap_byte(pixmap, x, y, w, h);
		break;
	case 15:
	case 16:
		_glamor_dump_pixmap_sword(pixmap, x, y, w, h);
		break;

	case 24:
	case 32:
		_glamor_dump_pixmap_word(pixmap, x, y, w, h);
		break;
	case 1:
		_glamor_dump_pixmap_bits(pixmap, x, y, w, h);
		break;
	default:
		ErrorF("dump depth %d, not implemented.\n", pixmap->drawable.depth);
	}
	glamor_finish_access(&pixmap->drawable, GLAMOR_ACCESS_RO);
}

static inline void _glamor_compare_pixmaps(PixmapPtr pixmap1, PixmapPtr pixmap2,
                                           int x, int y, int w, int h,
                                           PictFormatShort short_format,
                                           int all, int diffs)
{
	int i, j;
	unsigned char * p1 = pixmap1->devPrivate.ptr;
	unsigned char * p2 = pixmap2->devPrivate.ptr;
	int line_need_printed = 0;
	int test_code = 0xAABBCCDD;
	int little_endian = 0;
	unsigned char *p_test;
	int bpp = pixmap1->drawable.depth == 8 ? 1 : 4;
	int stride = pixmap1->devKind;

	assert(pixmap1->devKind == pixmap2->devKind);

	ErrorF("stride:%d, width:%d, height:%d\n", stride, w, h);

	p1 = p1 + y * stride + x;
	p2 = p2 + y * stride + x;

	if (all) {
		for (i = 0; i < h; i++) {
			ErrorF("line %3d: ", i);

			for (j = 0; j < stride; j++) {
				if (j % bpp == 0)
					ErrorF("[%d]%2x:%2x ", j / bpp, p1[j], p2[j]);
				else
					ErrorF("%2x:%2x ", p1[j], p2[j]);
			}

			p1 += stride;
			p2 += stride;
			ErrorF("\n");
		}
	} else {
		if (short_format == PICT_a8r8g8b8) {
			p_test = (unsigned char *) & test_code;
			little_endian = (*p_test == 0xDD);
			bpp = 4;

			for (i = 0; i < h; i++) {
				line_need_printed = 0;

				for (j = 0; j < stride; j++) {
					if (p1[j] != p2[j] && (p1[j] - p2[j] > diffs || p2[j] - p1[j] > diffs)) {
						if (line_need_printed) {
							if (little_endian) {
								switch (j % 4) {
									case 2:
										ErrorF("[%d]RED:%2x:%2x ", j / bpp, p1[j], p2[j]);
										break;
									case 1:
										ErrorF("[%d]GREEN:%2x:%2x ", j / bpp, p1[j], p2[j]);
										break;
									case 0:
										ErrorF("[%d]BLUE:%2x:%2x ", j / bpp, p1[j], p2[j]);
										break;
									case 3:
										ErrorF("[%d]Alpha:%2x:%2x ", j / bpp, p1[j], p2[j]);
										break;
								}
							} else {
								switch (j % 4) {
									case 1:
										ErrorF("[%d]RED:%2x:%2x ", j / bpp, p1[j], p2[j]);
										break;
									case 2:
										ErrorF("[%d]GREEN:%2x:%2x ", j / bpp, p1[j], p2[j]);
										break;
									case 3:
										ErrorF("[%d]BLUE:%2x:%2x ", j / bpp, p1[j], p2[j]);
										break;
									case 0:
										ErrorF("[%d]Alpha:%2x:%2x ", j / bpp, p1[j], p2[j]);
										break;
								}
							}
						} else {
							line_need_printed = 1;
							j = -1;
							ErrorF("line %3d: ", i);
							continue;
						}
					}
				}

				p1 += stride;
				p2 += stride;
				ErrorF("\n");
			}
		} //more format can be added here.
		else { // the default format, just print.
			for (i = 0; i < h; i++) {
				line_need_printed = 0;

				for (j = 0; j < stride; j++) {
					if (p1[j] != p2[j]) {
						if (line_need_printed) {
							ErrorF("[%d]%2x:%2x ", j / bpp, p1[j], p2[j]);
						} else {
							line_need_printed = 1;
							j = -1;
							ErrorF("line %3d: ", i);
							continue;
						}
					}
				}

				p1 += stride;
				p2 += stride;
				ErrorF("\n");
			}
		}
	}
}

static inline void glamor_compare_pixmaps(PixmapPtr pixmap1, PixmapPtr pixmap2,
                                          int x, int y, int w, int h, int all, int diffs)
{
	assert(pixmap1->drawable.depth == pixmap2->drawable.depth);

	glamor_prepare_access(&pixmap1->drawable, GLAMOR_ACCESS_RO);
	glamor_prepare_access(&pixmap2->drawable, GLAMOR_ACCESS_RO);

	_glamor_compare_pixmaps(pixmap1, pixmap2, x, y, w, h, -1, all, diffs);

	glamor_finish_access(&pixmap1->drawable, GLAMOR_ACCESS_RO);
	glamor_finish_access(&pixmap2->drawable, GLAMOR_ACCESS_RO);
}

/* This function is used to compare two pictures.
   If the picture has no drawable, we use fb functions to generate it. */
static inline void glamor_compare_pictures( ScreenPtr screen,
                                            PicturePtr fst_picture,
                                            PicturePtr snd_picture,
                                            int x_source, int y_source,
                                            int width, int height,
                                            int all, int diffs)
{
	PixmapPtr fst_pixmap;
	PixmapPtr snd_pixmap;
	int fst_generated, snd_generated;
	int error;
	int fst_type = -1;
	int snd_type = -1; // -1 represent has drawable.

	if (fst_picture->format != snd_picture->format) {
		ErrorF("Different picture format can not compare!\n");
		return;
	}

	if (!fst_picture->pDrawable) {
		fst_type = fst_picture->pSourcePict->type;
	}

	if (!snd_picture->pDrawable) {
		snd_type = snd_picture->pSourcePict->type;
	}

	if ((fst_type != -1) && (snd_type != -1) && (fst_type != snd_type)) {
		ErrorF("Different picture type will never be same!\n");
		return;
	}

	fst_generated = snd_generated = 0;

	if (!fst_picture->pDrawable) {
		PicturePtr pixman_pic;
		PixmapPtr pixmap = NULL;
		PictFormatShort format;

		format = fst_picture->format;

		pixmap = glamor_create_pixmap(screen,
		                              width, height,
		                              PIXMAN_FORMAT_DEPTH(format),
		                              GLAMOR_CREATE_PIXMAP_CPU);

		pixman_pic = CreatePicture(0,
		                           &pixmap->drawable,
		                           PictureMatchFormat(screen,
		                               PIXMAN_FORMAT_DEPTH(format), format),
		                           0, 0, serverClient, &error);

		fbComposite(PictOpSrc, fst_picture, NULL, pixman_pic,
		            x_source, y_source,
		            0, 0,
		            0, 0,
		            width, height);

		glamor_destroy_pixmap(pixmap);

		fst_picture = pixman_pic;
		fst_generated = 1;
	}

	if (!snd_picture->pDrawable) {
		PicturePtr pixman_pic;
		PixmapPtr pixmap = NULL;
		PictFormatShort format;

		format = snd_picture->format;

		pixmap = glamor_create_pixmap(screen,
		                              width, height,
		                              PIXMAN_FORMAT_DEPTH(format),
		                              GLAMOR_CREATE_PIXMAP_CPU);

		pixman_pic = CreatePicture(0,
		                           &pixmap->drawable,
		                           PictureMatchFormat(screen,
		                               PIXMAN_FORMAT_DEPTH(format), format),
		                           0, 0, serverClient, &error);

		fbComposite(PictOpSrc, snd_picture, NULL, pixman_pic,
		            x_source, y_source,
		            0, 0,
		            0, 0,
		            width, height);

		glamor_destroy_pixmap(pixmap);

		snd_picture = pixman_pic;
		snd_generated = 1;
	}

	fst_pixmap = glamor_get_drawable_pixmap(fst_picture->pDrawable);
	snd_pixmap = glamor_get_drawable_pixmap(snd_picture->pDrawable);

	if (fst_pixmap->drawable.depth != snd_pixmap->drawable.depth) {
		if (fst_generated)
			glamor_destroy_picture(fst_picture);
		if (snd_generated)
			glamor_destroy_picture(snd_picture);

		ErrorF("Different pixmap depth can not compare!\n");
		return;
	}

	glamor_prepare_access(&fst_pixmap->drawable, GLAMOR_ACCESS_RO);
	glamor_prepare_access(&snd_pixmap->drawable, GLAMOR_ACCESS_RO);

	if ((fst_type == SourcePictTypeLinear) ||
	     (fst_type == SourcePictTypeRadial) ||
	     (fst_type == SourcePictTypeConical) ||
	     (snd_type == SourcePictTypeLinear) ||
	     (snd_type == SourcePictTypeRadial) ||
	     (snd_type == SourcePictTypeConical)) {
		x_source = y_source = 0;
	}

	_glamor_compare_pixmaps(fst_pixmap, snd_pixmap,
	                        x_source, y_source,
	                        width, height,
	                        fst_picture->format, all, diffs);

	glamor_finish_access(&fst_pixmap->drawable, GLAMOR_ACCESS_RO);
	glamor_finish_access(&snd_pixmap->drawable, GLAMOR_ACCESS_RO);

	if (fst_generated)
		glamor_destroy_picture(fst_picture);
	if (snd_generated)
		glamor_destroy_picture(snd_picture);

	return;
}

static inline void glamor_make_current(ScreenPtr screen)
{
	glamor_egl_make_current(screen);
}

static inline void glamor_restore_current(ScreenPtr screen)
{
	glamor_egl_restore_context(screen);
}

#ifdef GLX_USE_SHARED_DISPATCH
static inline glamor_gl_dispatch *
glamor_get_dispatch(glamor_screen_private *glamor_priv)
{
	if (glamor_priv->flags & GLAMOR_USE_EGL_SCREEN)
		glamor_make_current(glamor_priv->screen);

	return &glamor_priv->_dispatch;
}

static inline void
glamor_put_dispatch(glamor_screen_private *glamor_priv)
{
	if (glamor_priv->flags & GLAMOR_USE_EGL_SCREEN)
		glamor_restore_current(glamor_priv->screen);
}
#else
#warning "Indirect GLX may be broken, need to implement context switch."
static inline glamor_gl_dispatch *
glamor_get_dispatch(glamor_screen_private *glamor_priv)
{
	return &glamor_priv->_dispatch;
}

static inline void
glamor_put_dispatch(glamor_screen_private *glamor_priv)
{
}

#endif

#endif
