CXX = g++
CXXFLAGS = -Wall -Wextra -I./$(INCLDIR)
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

PROG = $(BUILDDIR)/snake.elf
MODULES = snake textview graphview human model view
OBJ = $(addsuffix .o, $(MODULES))
SRC = $(addsuffix .cpp, $(MODULES))

BUILDDIR = build
SRCDIR = Src
INCLDIR = Include

all : $(PROG)

$(PROG) : $(addprefix $(BUILDDIR)/, $(OBJ))
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $@

$(addprefix $(BUILDDIR)/, $(OBJ)) : $(BUILDDIR)/%.o : $(SRCDIR)/%.cpp $(INCLDIR)/%.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
.PHONY: depend

clean : 
	$(RM) $(BUILDDIR)/*.o

depend : 
	$(CXX) $(CXXFLAGS) $(addprefix $(SRCDIR)/, $(SRC)) -MM > $(BUILDDIR)/.depend 

-include $(BUILDDIR)/.depend
