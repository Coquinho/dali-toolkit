/**
 * Copyright (c) 2014, Facebook, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 */

#include <stdio.h>
#include <string.h>

#include <dali-toolkit/third-party/facebook-flexbox/layout.h>

void test(const char *name, css_node_t *style, css_node_t *expected_layout);
bool tests_finished(void);
css_dim_t measure(void *context, float width, css_measure_mode_t widthMode, float height, css_measure_mode_t heightMode);
void init_css_node_children(css_node_t *node, int children_count);
css_node_t *new_test_css_node(void);
bool perform_layout_test(void);
