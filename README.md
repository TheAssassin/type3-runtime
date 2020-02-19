# AppImage type 3 runtime

This repository is a draft for a new AppImage type called *type 3*. It is
supposed to succeed the previous types, the deprecated type 1 and the
currently used type 2. Both have their flaws, and we've learnt a lot in the
last years working with both.


## Design goals

- Clarity on licensing: Properly licensed runtime which can be safely used
  by third party projects making sure all libraries in our dependency tree
  are used in compliance with their licenses
- Minimum size: Like the type 2 runtime, the new type should also use < 1 MiB
  of size, ideally way less
- Ensure strict compliance to the ELF standard: The magic bytes need to be
  embedded in a non-offending way to eliminate several problems, e.g., when
  running AppImages in Docker containers and to eliminate the need for
  [appimagelauncherfs](https://github.com/TheAssassin/AppImageLauncher)
- Support for [AppImageUpdate](https://github.com/AppImage/AppImageUpdate)
- Improved signing (like, multiple signatures, easier verification)
- Built in integrity checks (to make sure chopped off or otherwise broken
  AppImages will produce a proper error message)
- Support different image formats (squashfs remaining the default for now)
- Reduced attack surface (minimize amounts of third-party code)
- Fully static builds with only the kernel (and FUSE) as dependency
  (so that the runtime itself works on glibc, musl etc. systems)
- Provide alternative implementations made in different languages (to i
- ... more goals might be added in the future


## AppImage type 3 architecture

The basic structure of type 2 shall be used for type 3, too. Type 2 uses the
following file structure:

```
+------------------+
| AppImage runtime |
|   (ELF binary)   |
+------------------+
|                  |
|     payload      |
| (squashfs image) |
|                  |
.                  .
.                  .
.                  .
+------------------+
```

This approach is very simple to implement and use, as we've seen in the
previous AppImage types. The runtime is an ELF binary, and the advanced
features AppImages provide (updating, signing) rely on so-called ELF
sections, which are defined by the AppImage standard. There are several
problems related to them, though. Not only they are somewhat complex to
read from applications (you need to use complex headers like `elf.h` and
need to understand the ELF standard to some extent), they're also
complicated to create in an ELF file. The sections in use right now are
created during the build of the runtime and can't be changed later on.

It might make sense to introduce a third entity in the AppImage in between
the ELF runtime and the payload, a kind of *AppImage header*. This way, the
runtime does not have to be patched at all during build, the upstream binary
can be used directly. Instead of writing to ELF sections, this AppImage header
can be generated from the auxiliary data by the AppImage build tool (i.e.,
the future `appimagetool`).

Being able to use an unmodified runtime also provides the following benefits:

- The runtime is exchangable without losing any functionality, as the AppImage
  meta header is not needed by it
- The runtime can be signed officially by the AppImage project and this
  signature can be validated by third parties

The new AppImage file format might therefore look like this:

```
+------------------+
| AppImage runtime |
|   (ELF binary)   |
+------------------+
| AppImage header  |  <-- new
+------------------+
|                  |
|     payload      |
| (squashfs image) |
|                  |
.                  .
.                  .
.                  .
+------------------+
```


## Development roadmap

At first, a prototype shall be implemented in plain C, using squashfuse
internally, just like type 2. However, it should be written from scratch and
support said AppImage header.
The prototype does not yet have to be linked fully statically
It should work with existing type 2 AppImage payloads.
It should no longer be a single code file and should support unit testing.

The next step should be to evaluate alternative languages. Rust is an
interesting candidate, as it helps write safer code on the language level
already.

Next, the runtime size should be minimized, e.g., by trimming down
dependencies or writing dependencies ourselves. At the same time, the
licensing problems mentioned in the design goals should be resolved.

Then, static linking should be evaluated.
