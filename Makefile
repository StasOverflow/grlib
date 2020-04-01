BUILD_DIR = build
OBJ_DIR   = build/obj
LIB_DIR   = build/lib

FTRASTERIZE      = $(EMLIB)/bin/ftrasterize_v2.0

FONTS_DATA_DIR   = fonts_data
FONTS_DIR        = fonts

#
# Include the common make definitions.
#
include ./makedefs

#
# Where to find source files that do not live in this directory.
#
VPATH  = .                 \
         ${FONTS_DIR}      \
         ${FONTS_DATA_DIR} \
         widgets

#
# Where to find header files that do not live in the source directory.
#
IPATH  = ..      \
         .       \
         widgets

#
# The default rule, which causes the graphics library to be built.
#
all:              \
     ${BUILD_DIR} \
     ${OBJ_DIR}   \
     ${LIB_DIR}   \
     ${LIB_DIR}/libgr-impuls.a

#
# The rule to clean out all the build products.
#
clean:
	@echo -n "  Cleaning... "
	@rm -rf ${BUILD_DIR}
	@echo "Done."

#
# The rule to create the target directory.
#
${BUILD_DIR}:
	@mkdir -p ${BUILD_DIR}

${OBJ_DIR}:
	@mkdir -p ${OBJ_DIR}

${LIB_DIR}:
	@mkdir -p ${LIB_DIR}



#
# Rules for building the graphics library.
#
${LIB_DIR}/libgr-impuls.a:  ${OBJ_DIR}/Tahoma12pt.o ${FONTS_DIR}/Tahoma12pt.c
${LIB_DIR}/libgr-impuls.a:  ${OBJ_DIR}/Tahoma13pt.o ${FONTS_DIR}/Tahoma13pt.c

${LIB_DIR}/libgr-impuls.a:  ${OBJ_DIR}/Doslike15pt.o ${FONTS_DIR}/Doslike15pt.c

${LIB_DIR}/libgr-impuls.a:  ${OBJ_DIR}/Courier19pt.o ${FONTS_DIR}/Courier19pt.c
${LIB_DIR}/libgr-impuls.a:  ${OBJ_DIR}/Courier21pt.o ${FONTS_DIR}/Courier21pt.c

${LIB_DIR}/libgr-impuls.a:  ${OBJ_DIR}/Times20pt.o ${FONTS_DIR}/Times20pt.c
${LIB_DIR}/libgr-impuls.a:  ${OBJ_DIR}/Times22pt.o ${FONTS_DIR}/Times22pt.c
${LIB_DIR}/libgr-impuls.a:  ${OBJ_DIR}/Times25pt.o ${FONTS_DIR}/Times25pt.c


${LIB_DIR}/libgr-impuls.a:  ${OBJ_DIR}/Digital09pt.o ${FONTS_DIR}/Digital09pt.c
${LIB_DIR}/libgr-impuls.a:  ${OBJ_DIR}/Digital23pt.o ${FONTS_DIR}/Digital23pt.c
${LIB_DIR}/libgr-impuls.a:  ${OBJ_DIR}/Digital53pt.o ${FONTS_DIR}/Digital53pt.c
${LIB_DIR}/libgr-impuls.a:  ${OBJ_DIR}/Digital54pt.o ${FONTS_DIR}/Digital54pt.c


${LIB_DIR}/libgr-impuls.a: ${OBJ_DIR}/context.o
${LIB_DIR}/libgr-impuls.a: ${OBJ_DIR}/circle.o
${LIB_DIR}/libgr-impuls.a: ${OBJ_DIR}/line.o
${LIB_DIR}/libgr-impuls.a: ${OBJ_DIR}/rectangle.o
${LIB_DIR}/libgr-impuls.a: ${OBJ_DIR}/image.o
${LIB_DIR}/libgr-impuls.a: ${OBJ_DIR}/charmap.o
${LIB_DIR}/libgr-impuls.a: ${OBJ_DIR}/string.o
${LIB_DIR}/libgr-impuls.a: ${OBJ_DIR}/timer.o
${LIB_DIR}/libgr-impuls.a: ${OBJ_DIR}/mqueue.o
${LIB_DIR}/libgr-impuls.a: ${OBJ_DIR}/tree.o

${LIB_DIR}/libgr-impuls.a: ${OBJ_DIR}/widget.o
${LIB_DIR}/libgr-impuls.a: ${OBJ_DIR}/canvas.o
${LIB_DIR}/libgr-impuls.a: ${OBJ_DIR}/rectangularbutton.o
${LIB_DIR}/libgr-impuls.a: ${OBJ_DIR}/circularbutton.o
#${LIB_DIR}/libgr-impuls.a: ${OBJ_DIR}/imgbutton.o
#${LIB_DIR}/libgr-impuls.a: ${OBJ_DIR}/radiobutton.o
#${LIB_DIR}/libgr-impuls.a: ${OBJ_DIR}/checkbox.o
#${LIB_DIR}/libgr-impuls.a: ${OBJ_DIR}/container.o
#${LIB_DIR}/libgr-impuls.a: ${OBJ_DIR}/listbox.o
#${LIB_DIR}/libgr-impuls.a: ${OBJ_DIR}/slider.o

#${LIB_DIR}/libgr-impuls.a: ${OBJ_DIR}/offscr1bpp.o
#${LIB_DIR}/libgr-impuls.a: ${OBJ_DIR}/offscr4bpp.o
#${LIB_DIR}/libgr-impuls.a: ${OBJ_DIR}/offscr8bpp.o
