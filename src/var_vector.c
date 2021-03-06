//
//  var_vector.c
//  cadet
//
//  Created by Markus Rabe on 20/11/2016.
//  Copyright © 2016 UC Berkeley. All rights reserved.
//

#include "var_vector.h"
#include "qcnf.h"

#include "vector.h"
#include "log.h"
#include "stdbool.h"
#include "util.h"

#include <assert.h>

void var_vector_increase(var_vector* v) {
    v->size *= INCREASE_FACTOR;
    v->data = realloc(v->data, sizeof(Var) * v->size);
    //    Var* newdata = malloc(sizeof(Var) * v->size);
    //    for (unsigned i = 0; i < v->count; i++) {
    //        newdata[i] = v->data[i];
    //    }
    //    free(v->data);
    //    v->data = newdata;
}

// assumes it can write to the memory location
void var_vector_init_struct(var_vector* v) {
    assert(v);
    v->data = malloc(sizeof(Var) * INITIAL_SIZE);
    v->count = 0;
    v->size = INITIAL_SIZE;
}

var_vector* var_vector_init() {
    var_vector* v = malloc(sizeof(var_vector));
    var_vector_init_struct(v);
    return v;
}

void var_vector_resize(var_vector* v, unsigned size) {
    assert(size >= v->count);
    v->data = realloc(v->data, size * sizeof(Var));
    v->size = size;
}

void var_vector_reset(var_vector* v) {
    v->count = 0;
}

void var_vector_free(var_vector* v) {
    free(v->data);
    free(v);
}

unsigned var_vector_count(var_vector* v) {
    return v->count;
}

void var_vector_reduce_count(var_vector* v, unsigned j) {
    assert(j <= v->count);
    v->count = j;
}

Var* var_vector_get(var_vector* v, unsigned i) {
    assert (v->count > i);
    return &v->data[i];
}

void var_vector_set(var_vector* v, unsigned i, Var value) {
    assert (v->count > i);
    v->data[i] = value;
}

void var_vector_add(var_vector* v, Var value) {
    if (v->size == v->count) {
        V4("Warning: Resizing variable vector. Variable pointers may be invalidated!\n");
        var_vector_increase(v);
    }
    v->data[v->count] = value;
    v->count += 1;
}

void var_vector_print(var_vector* v) {
    V4("var_vector (%u,%u) ", v->count, v->size);
    for (unsigned j = 0; j < v->count; j++) {
        Var* var = &v->data[j];
        V1(" %d", var->var_id);
    }
    V1("\n");
}

void var_vector_remove_index(var_vector* v, unsigned i) {
    v->count = v->count - 1; // yes, before the loop
    for (; i < v->count; i++) {
        v->data[i] = v->data[i+1];
    }
}

void var_vector_sort(var_vector* v, int (*cmp)(const void*, const void*)) {
    qsort(v->data, v->count, sizeof(Var), cmp);
}

var_vector* var_vector_copy(var_vector* old) {
    var_vector* new = var_vector_init();
    for (unsigned i = 0; i < var_vector_count(old); i++) {
        var_vector_add(new, *var_vector_get(old, i));
    }
    return new;
}

void var_vector_add_all(var_vector* this, var_vector* other) {
    for (unsigned i = 0; i < var_vector_count(other); i++) {
        var_vector_add(this, *var_vector_get(other, i));
    }
}

void var_vector_remove_last_element(var_vector* v) {
    assert(v->count > 0);
    v->count = v->count - 1;
}

