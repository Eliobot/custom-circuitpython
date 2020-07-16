/*
 * This file is part of the Micro Python project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2020 Scott Shawcroft for Adafruit Industries
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "shared-bindings/memorymonitor/__init__.h"
#include "shared-bindings/memorymonitor/AllocationAlarm.h"

#include "py/gc.h"
#include "py/mpstate.h"
#include "py/runtime.h"

void common_hal_memorymonitor_allocationalarm_construct(memorymonitor_allocationalarm_obj_t* self, size_t minimum_block_count) {
    self->minimum_block_count = minimum_block_count;
    self->next = NULL;
    self->previous = NULL;
}

void common_hal_memorymonitor_allocationalarm_pause(memorymonitor_allocationalarm_obj_t* self) {
    *self->previous = self->next;
    self->next = NULL;
    self->previous = NULL;
}

void common_hal_memorymonitor_allocationalarm_resume(memorymonitor_allocationalarm_obj_t* self) {
    if (self->previous != NULL) {
        mp_raise_RuntimeError(translate("Already running"));
    }
    self->next = MP_STATE_VM(active_allocationalarms);
    self->previous = (memorymonitor_allocationalarm_obj_t**) &MP_STATE_VM(active_allocationalarms);
    if (self->next != NULL) {
        self->next->previous = &self->next;
    }
    MP_STATE_VM(active_allocationalarms) = self;
}

void memorymonitor_allocationalarms_allocation(size_t block_count) {
    memorymonitor_allocationalarm_obj_t* alarm = MP_OBJ_TO_PTR(MP_STATE_VM(active_allocationalarms));
    size_t alert_count = 0;
    while (alarm != NULL) {
        // Hold onto next in case we remove the alarm from the list.
        memorymonitor_allocationalarm_obj_t* next = alarm->next;
        if (block_count >= alarm->minimum_block_count) {
            common_hal_memorymonitor_allocationalarm_pause(alarm);
            alert_count++;
        }
        alarm = next;
    }
    if (alert_count > 0) {
        mp_raise_memorymonitor_AllocationError(translate("Attempt to allocate %d blocks"), block_count);
    }
}
