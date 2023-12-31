/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef MESON_DRM_PLANE_H
#define MESON_DRM_PLANE_H

enum meson_drm_scaling_filter {
	DRM_SCALING_FILTER_BICUBIC_SHARP = 2,
	DRM_SCALING_FILTER_BICUBIC,
	DRM_SCALING_FILTER_BILINEAR,
	DRM_SCALING_FILTER_2POINT_BILINEAR,
	DRM_SCALING_FILTER_3POINT_TRIANGLE_SHARP,
	DRM_SCALING_FILTER_3POINT_TRIANGLE,
	DRM_SCALING_FILTER_4POINT_TRIANGLE,
	DRM_SCALING_FILTER_4POINT_BSPLINE,
	DRM_SCALING_FILTER_3POINT_BSPLINE,
	DRM_SCALING_FILTER_REPEATE,
};

#endif
