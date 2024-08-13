#ifndef X86_H
#define X86_H

#include "types.h"

static inline void outb(uint16_t port, uint8_t value) {
    asm volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}

static inline void outw(uint16_t port, uint16_t value) {
    asm volatile ("outw %0, %1" : : "a"(value), "Nd"(port));
}

static inline void outl(uint16_t port, uint32_t value) {
    asm volatile ("outl %0, %1" : : "a"(value), "Nd"(port));
}

static inline uint8_t inb(uint16_t port) {
    uint8_t value;
    asm volatile ("inb %1, %0" : "=a"(value) : "Nd"(port));
    return value;
}

static inline uint16_t inw(uint16_t port) {
    uint16_t value;
    asm volatile ("inw %1, %0" : "=a"(value) : "Nd"(port));
    return value;
}

static inline uint32_t inl(uint16_t port) {
    uint32_t value;
    asm volatile ("inl %1, %0" : "=a"(value) : "Nd"(port));
    return value;
}

static inline void disable_interrupts() {
    __asm__ __volatile__ ("cli" ::: "memory");
}

static inline void enable_interrupts() {
    __asm__ __volatile__ ("sti" ::: "memory");
}

static inline void memset(void *address, uint8_t value, size_t size) {
    uint8_t *ptr = (uint8_t *)address;
    for (size_t i = 0; i < size; i++) {
        ptr[i] = value; 
    }
}

#endif