#pragma once

static int FingersErrorMask[] = {
    0x80000000, // Thumb DIP Error Bit
    0x40000000, // Thumb PIP Error Bit
    0x20000000, // Thumb MCP Error Bit
    0x10000000, // Thumb SWP Error Bit
    0x8000000,  // Thumb ROP Error Bit
    0x4000000,  // Index DIP Error Bit
    0x2000000,  // Index PIP Error Bit
    0x1000000,  // Index MCP Error Bit
    0x800000,   // Index SWP Error Bit
    0x400000,   // Middle DIP Error Bit
    0x200000,   // Middle PIP Error Bit
    0x100000,   // Middle MCP Error Bit
    0x80000,    // Middle SWP Error Bit
    0x40000,    // Ring DIP Error Bit
    0x20000,    // Ring PIP Error Bit
    0x10000,    // Ring MCP Error Bit
    0x8000,     // Ring SWP Error Bit
    0x4000,     // Little DIP Error Bit
    0x2000,     // Little PIP Error Bit
    0x1000,     // Little MCP Error Bit
    0x800,      // Little SWP Error Bit
    0x80,       // Thumb Vide Motor Error Bit
    0x40,       // Index Vide Motor Error Bit
    0x20,       // Middle Vide Motor Error Bit
    0x10,       // Ring Vide Motor Error Bit
    0x8,        // Little Vide Motor Error Bit
};