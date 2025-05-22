#ifndef TYPES_H
#define TYPES_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/// Exports
/// -------
/// * size_t
/// * ptrdiff_t
/// * bool, also: true, false.

typedef char* cstring;
typedef struct { bool ok; } Status;

Status ok() {
    return (Status) { .ok = true };
}

Status err() {
    return (Status) { .ok = false };
}

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

#define nil (NULL)

#endif