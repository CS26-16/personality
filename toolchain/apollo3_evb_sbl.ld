/* Apollo3 (1MB flash, 384KB SRAM) with SBL (bootloader) offset.
   Flash base: 0x00000000
   SBL reserved: 0x0000C000 (48KB)
   App flash:   0x0000C000 .. 0x000FFFFF
   SRAM:        0x10000000 .. 0x1005FFFF (384KB)
*/

MEMORY
{
  FLASH (rx)  : ORIGIN = 0x0000C000, LENGTH = 0x100000 - 0x0000C000  /* 0xF4000 bytes */
  SRAM  (rwx) : ORIGIN = 0x10000000, LENGTH = 0x60000                /* 384 KB */
}

/* Symbols some SDKs expect */
_estack = ORIGIN(SRAM) + LENGTH(SRAM);

SECTIONS
{
  /* Vector table */
  .isr_vector :
  {
    KEEP(*(.isr_vector))
  } > FLASH

  /* Code + rodata */
  .text :
  {
    *(.text*) *(.glue_7) *(.glue_7t)
    *(.rodata*) *(.constdata*)
    KEEP(*(.init)) KEEP(*(.fini))
  } > FLASH

  /* C++ static constructors/destructors */
  .preinit_array :
  {
    PROVIDE_HIDDEN(__preinit_array_start = .);
    KEEP(*(.preinit_array*))
    PROVIDE_HIDDEN(__preinit_array_end = .);
  } > FLASH

  .init_array :
  {
    PROVIDE_HIDDEN(__init_array_start = .);
    KEEP(*(SORT_BY_INIT_PRIORITY(.init_array.*) SORT_BY_INIT_PRIORITY(.ctors.*)))
    KEEP(*(.init_array*))
    PROVIDE_HIDDEN(__init_array_end = .);
  } > FLASH

  .fini_array :
  {
    PROVIDE_HIDDEN(__fini_array_start = .);
    KEEP(*(SORT_BY_INIT_PRIORITY(.fini_array.*) SORT_BY_INIT_PRIORITY(.dtors.*)))
    KEEP(*(.fini_array*))
    PROVIDE_HIDDEN(__fini_array_end = .);
  } > FLASH

  /* Initialized data: LMA in FLASH, VMA in SRAM */
  .data : AT (ADDR(.text) + SIZEOF(.text))
  {
    __data_start__ = .;
    *(.data*)
    *(.ramfunc*)                   /* if you place functions in RAM */
    . = ALIGN(4);
    __data_end__ = .;
  } > SRAM
  __data_load__ = LOADADDR(.data);

  /* Uninitialized data */
  .bss (NOLOAD) :
  {
    __bss_start__ = .;
    *(.bss*) *(COMMON)
    . = ALIGN(4);
    __bss_end__ = .;
  } > SRAM

  /* Heap and stack */
  .heap (NOLOAD) :
  {
    __end__ = .;
    end = __end__;
    *(.heap*)
  } > SRAM

  .stack (NOLOAD) :
  {
    __stack_start__ = .;
    . = ORIGIN(SRAM) + LENGTH(SRAM);
    __stack_end__ = .;
  } > SRAM
}

/* Useful aliases */
PROVIDE(__StackTop = _estack);
PROVIDE(__StackLimit = ORIGIN(SRAM));
