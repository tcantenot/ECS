SRC_DIR = ./
OBJ_DIR = ./obj/
DEP_DIR = ./dep/
HDR_DIR = ./ ../include/
BIN_DIR = ./bin

SOURCES = $(shell find $(SRC_DIR) -type f -name \*.cpp)
OBJECTS = $(subst $(SRC_DIR), $(OBJ_DIR), $(SOURCES:%.cpp=%.o))
DEPS    = $(subst $(OBJ_DIR), $(DEP_DIR), $(OBJECTS:%.o=%.d))

BINARY = tests 
TARGET = $(BIN_DIR)/$(BINARY)
STATIC_LIB = ../bin/hybrid_ecs.a

WARNINGS.cpp.debug  = -W -Wall -Wextra -pedantic -Wold-style-cast -Woverloaded-virtual \
				   -Wfloat-equal -Wwrite-strings -Wpointer-arith -Wcast-align \
				   -Wconversion -Wredundant-decls -Winit-self -Wswitch-default \
				   -Wswitch-enum -Wundef -Winline -Wunused-parameter -Weffc++ #-Werror -Wno-error=sign-compare

WARNINGS =#$(WARNINGS.cpp.debug)

ALL     = all
DEBUG   = debug
RELEASE = release
FAST 	= fast
PROFILE = profile
RUN     = run
GPROF   = gprof
CLEAN   = clean

GMON_FILE = ./gmon.out


HDR_FLAGS = $(foreach dir, $(HDR_DIR), -I$(dir))
CFLAGS  = -std=c++11 $(WARNINGS)
LDFLAGS = -std=c++11 $(shell sdl2-config --cflags) $(shell sdl2-config --libs) -lSDL2_image 

$(OBJ_DIR)%.o : $(SRC_DIR)%.cpp
	@echo "Compiling < $< >..."
	@mkdir -p $(@D)
	@mkdir -p $(dir $(DEP_DIR)$*.d)
	g++ $(CFLAGS) $(HDR_FLAGS) -c $< -MMD -MF $(DEP_DIR)$*.d -o $@ 

 
$(RELEASE): CFLAGS  += -DNDEBUG -O3
$(RELEASE): LDFLAGS += -O3
$(RELEASE): $(ALL)

$(ALL): $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	g++ $(LDFLAGS) $^ $(STATIC_LIB) -o $(TARGET)

$(RUN): 
	$(TARGET)

$(DEBUG): CFLAGS  += -g3 -O0 -pg
$(DEBUG): LDFLAGS += -O0 -pg
$(DEBUG): WARNINGS = $(WARNINGS.cpp.debug)
$(DEBUG): $(ALL)

$(PROFILE): CFLAGS  += -g3 -O0 -pg
$(PROFILE): LDFLAGS += -O0 -pg
$(PROFILE): $(ALL)

$(GPROF):
	$(GPROF) $(TARGET) $(GMON_FILE) | less

$(CLEAN):
	rm -r $(OBJECTS) $(DEPS) $(TARGET) 2> /dev/null
