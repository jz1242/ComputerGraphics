TARGET = Ray
SOURCE = glew.c GLSLProgram.cpp mouse.cpp mouse.cpp rayCamera.cpp rayCone.todo.cpp rayDirectionalLight.cpp rayGroup.cpp rayPointLight.cpp rayScene.todo.cpp raySpotLight.cpp rayTriangle.todo.cpp rayBox.cpp rayCamera.todo.cpp rayCylinder.cpp rayDirectionalLight.todo.cpp rayGroup.todo.cpp rayPointLight.todo.cpp raySphere.cpp raySpotLight.todo.cpp rayWindow.cpp rayBox.todo.cpp rayCone.cpp rayCylinder.todo.cpp rayFileInstance.cpp rayKey.cpp rayScene.cpp raySphere.todo.cpp rayTriangle.cpp

TARGET_NO_GL = RayNoGL
SOURCE_NO_GL = mouse.cpp mouse.cpp rayCamera.cpp rayCone.todo.cpp rayDirectionalLight.cpp rayGroup.cpp rayPointLight.cpp rayScene.todo.cpp raySpotLight.cpp rayTriangle.todo.cpp rayBox.cpp rayCamera.todo.cpp rayCylinder.cpp rayDirectionalLight.todo.cpp rayGroup.todo.cpp rayPointLight.todo.cpp raySphere.cpp raySpotLight.todo.cpp rayBox.todo.cpp rayCone.cpp rayCylinder.todo.cpp rayFileInstance.cpp rayKey.cpp rayScene.cpp raySphere.todo.cpp rayTriangle.cpp


TARGET_LIB = lib$(TARGET).a
TARGET_NO_GL_LIB = lib$(TARGET_NO_GL).a

CFLAGS += -I. -I.. -Wunused-result
CPPFLAGS += -I. -I.. -std=c++14 -Wunused-result
CFLAGS_NO_GL = -DNO_OPEN_GL

CPPFLAGS_DEBUG = -DDEBUG -g3
CPPFLAGS_RELEASE = -O3 -DRELEASE -funroll-loops -ffast-math -DNDEBUG

SRC = ./
BIN = ../
BIN_O = ../Bin/Linux/Release/$(TARGET)/
INCLUDE = /usr/include/

CC  = gcc
CXX = g++
MD  = mkdir
AR  = ar

OBJECTS=$(addprefix $(BIN_O), $(addsuffix .o, $(basename $(SOURCE))))
OBJECTS_NO_GL=$(addprefix $(BIN_O), $(addsuffix .o, $(basename $(SOURCE_NO_GL))))

all: CFLAGS += $(CFLAGS_RELEASE)
all: CPPFLAGS += $(CFLAGS_RELEASE)
all: $(BIN)
all: $(BIN_O)
all: $(BIN)$(TARGET_LIB)

debug: CFLAGS += $(CFLAGS_DEBUG)
debug: CPPFLAGS += $(CFLAGS_DEBUG)
debug: $(BIN)
debug: $(BIN)$(TARGET_LIB)

allNoGL: CFLAGS += $(CFLAGS_RELEASE)
allNoGL: CPPFLAGS += $(CFLAGS_RELEASE)
allNoGL: CFLAGS += $(CFLAGS_NO_GL)
allNoGL: CPPFLAGS += $(CFLAGS_NO_GL)
allNoGL: $(BIN)
allNoGL: $(BIN_O)
allNoGL: $(BIN)$(TARGET_NO_GL_LIB)

debugNoGL: CFLAGS += $(CFLAGS_DEBUG)
debugNoGL: CPPFLAGS += $(CFLAGS_DEBUG)
debugNoGL: CFLAGS += $(CFLAGS_NO_GL)
debugNoGL: CPPFLAGS += $(CFLAGS_NO_GL)
debugNoGL: $(BIN)
debugNoGL: $(BIN)$(TARGET_NO_GL_LIB)

clean:
	rm -f $(BIN)$(TARGET_LIB)
	rm -f $(OBJECTS)

$(BIN):
	$(MD) -p $(BIN)

$(BIN_O):
	$(MD) -p $(BIN_O)

$(BIN)$(TARGET_LIB): $(OBJECTS)
	$(AR) rcs $@ $(OBJECTS)

$(BIN)$(TARGET_NO_GL_LIB): $(OBJECTS_NO_GL)
	$(AR) rcs $@ $(OBJECTS_NO_GL)

$(BIN_O)%.o: $(SRC)%.c
	$(CC) -c -o $@ $(CFLAGS) -I$(INCLUDE) $<

$(BIN_O)%.o: $(SRC)%.cpp
	$(CXX) -c -o $@ $(CPPFLAGS) -I$(INCLUDE) $<
