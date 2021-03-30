COMPONENT_NAME = power

SRC_FILES = \
	../devices/abov/a33g/pwr.c \

TEST_SRC_FILES = \
	src/devices/abov/a33g/pwr_test.cpp \
	src/test_all.cpp \

INCLUDE_DIRS = \
	../include \
	stubs/overrides \
	$(CPPUTEST_HOME)/include \

CPPUTEST_CPPFLAGS = \
	-DDEVICE=a33g \
	-Darmv7m3 \
	-DA33G52x_H \
	-DUNITTEST \
	-include a33g.h

include MakefileRunner.mk
