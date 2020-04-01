CLANG_FORMAT = clang-format
CMAKE = cmake

BUILDDIR = build
CMAKEFLAGS =

.PHONY: all
all: debug

.PHONY: debug
debug:
	$(MAKE) build BUILD_TYPE=Debug BUILDDIR=build/$@

.PHONY: release
release:
	$(MAKE) build BUILD_TYPE=Release BUILDDIR=build/$@

.PHONY: build
build:
	mkdir -p $(BUILDDIR)
	cd $(BUILDDIR) && $(CMAKE) -DCMAKE_BUILD_TYPE=$(BUILD_TYPE) $(CMAKEFLAGS) $(CURDIR)
	$(MAKE) -C $(BUILDDIR)

.PHONY: clean
clean:
	$(RM) -R $(BUILDDIR)

.PHONY: format
format:
	find . -type f \( -name '*.cpp' -o -name '*.h' \) -print0 \
		| xargs -0 $(CLANG_FORMAT) -i
