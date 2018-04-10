# Questions

## What's `stdint.h`?

The `stdint.h` library is, according to [this documentation](https://en.wikibooks.org/wiki/C_Programming/stdint.h):
> _"stdint.h is a header file in the C standard library introduced in the C99 standard library section 7.18 to allow programmers to write more portable code by providing a set of typedefs that specify exact-width integer types, together with the defined minimum and maximum allowable values for each type, using macros[1]"_

In other words, this library allows us to specify data types that are of a specified width in memory. This is useful when working with standardized file formats, such as `.bmp`, which expect integers between 0 and 255. Since color in a `.bmp` is represented as red-green-blue (stored as blue-green-red on disk), each hue's intensity is determined by the integer between 0 and 255. By using `stdint.h` and creating an unsigned integer (named, by convention, `uint8_t`), we can specify data types that are already (and always) a certain width - therefore ensuring, in the case of an 8-bit/1-byte integers used to determine hue intensity, the integers are never negative or over 255. 

## What's the point of using `uint8_t`, `uint32_t`, `int32_t`, and `uint16_t` in a program?

As mentioned above, the point of using `uint8_t` et al. is that we are pre-specifying the memory width of the integers, and thus specifying their range. If we know that we never want our hue intensity values to be over 255, for example, we know that we should use an `uint8_t` data type. They also allow us to not have to use padding bits on our `.bmp` scanlines when , for example, the `.bmp` file size is not a multiple of 4. 

## How many bytes is a `BYTE`, a `DWORD`, a `LONG`, and a `WORD`, respectively?

From `bmp.h`:

```{c}
typedef uint8_t  BYTE;
typedef uint32_t DWORD;
typedef int32_t  LONG;
typedef uint16_t WORD;
```

This means that:
- `BYTE` is 8 bits, or 1 byte.
- `DWORD` is 32 bits, or 4 bytes.
- `LONG` is 32 bits, or 4 bytes.
- `WORD` is 16 bits, or 2 bytes.


## What (in ASCII, decimal, or hexadecimal) must the first two bytes of any BMP file be? Leading bytes used to identify file formats (with high probability) are generally called "magic numbers."

The first two bytes of any `.bmp` file are (or should be) the `BITMAPFILEHEADER`'s first element, which is a `WORD` type variable called `bfType`. Its value should be set to `BM`, which is:
- `ASCII`: `BM`
- Decimal: 66 77
- Hexadecimal: 42 4d

## What's the difference between `bfSize` and `biSize`?

According to the [Microsoft docs](https://msdn.microsoft.com/en-us/library/dd183374(v=vs.85).aspx): `bfSize` is _"The size, in bytes, of the bitmap file."_. For `biSize`, the [docs](https://msdn.microsoft.com/en-us/library/dd183376(v=vs.85).aspx) define it as _"The number of bytes required by the structure."_. In other words, `bfSize` refers to the data needed to store the entire file - including `BITMAPFILEHEADER`, `BITMAPINFOHEADER`, and the actual image. `biSize`, instead, is the data needed to store `BITMAPINFOHEADER` only. It should be fixed, while the `biSizeImage` (the size of the image), may vary.

## What does it mean if `biHeight` is negative?

In this case, it means the image is "flipped". From the [docs](https://msdn.microsoft.com/en-us/library/dd183376(v=vs.85).aspx):
>_"If biHeight is negative, the bitmap is a top-down DIB and its origin is the upper-left corner."_

## What field in `BITMAPINFOHEADER` specifies the BMP's color depth (i.e., bits per pixel)?

`biBitCount` determines the bits per pixel. 

## Why might `fopen` return `NULL` in lines 24 and 32 of `copy.c`?

If no file exists of that name , then the pointer which `fopen()` returns will be `NULL`. `fopen()` returns `NULL` when it fails.

## Why is the third argument to `fread` always `1` in our code?

TODO

## What value does line 63 of `copy.c` assign to `padding` if `bi.biWidth` is `3`?

TODO

## What does `fseek` do?

TODO

## What is `SEEK_CUR`?

TODO
