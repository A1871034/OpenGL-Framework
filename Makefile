CC =g++
BASE_DIR =$(CURDIR)
CFLAGS =-I "$(BASE_DIR)/dependancies/include" -I "$(BASE_DIR)/src/vendor"
LFLAGS =-L "$(BASE_DIR)/dependancies/lib" -lglfw3 -lopengl32 -lgdi32 -lglew32
SRC_DIR =src
COMPILE_DIR  =compile
VENDORS = stb_image imgui imgui_demo imgui_draw imgui_tables imgui_widgets imgui_impl_opengl3 imgui_impl_glfw
OBJECTS = main core IndexBuffer VertexBuffer VertexArray VertexBufferLayout Shader Renderer Texture
EXECUTABLE =main.exe

$(EXECUTABLE) : $(OBJECTS:%=$(COMPILE_DIR)/%.o)
	g++ -g $(OBJECTS:%=$(COMPILE_DIR)/%.o) $(VENDORS:%=$(COMPILE_DIR)/vendor/%.o) -o "$(BASE_DIR)/$(EXECUTABLE)" $(LFLAGS)

$(COMPILE_DIR)/%.o: $(SRC_DIR)/%.cpp $(SRC_DIR)/%.h
	$(CC) -g $(CFLAGS) -c "$<" -o "$@"

# Will only trigger this rule if the .h file doesn't exist
$(SRC_DIR)/%.h: ;


# Recompile vendor stuff
vendors: $(VENDORS:%=$(COMPILE_DIR)/vendor/%.o)

$(COMPILE_DIR)/vendor/%.o: $(SRC_DIR)/vendor/*/%.cpp $(SRC_DIR)/vendor/*/%.h
	$(CC) -g $(CFLAGS) -c "$<" -o "$@"