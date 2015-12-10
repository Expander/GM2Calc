DIR          := src
MODNAME      := gm2calc

# source files for library
LIBsrc_SRC := \
		$(DIR)/dilog.cpp \
		$(DIR)/ffunctions.cpp \
		$(DIR)/gm2_1loop.cpp \
		$(DIR)/gm2_1loop_c.cpp \
		$(DIR)/gm2_2loop.cpp \
		$(DIR)/gm2_2loop_c.cpp \
		$(DIR)/gm2_mb.cpp \
		$(DIR)/gm2_uncertainty.cpp \
		$(DIR)/gm2_slha_io.cpp \
		$(DIR)/MSSMNoFV_onshell.cpp \
		$(DIR)/MSSMNoFV_onshell_c.cpp \
		$(DIR)/MSSMNoFV_onshell_mass_eigenstates.cpp \
		$(DIR)/MSSMNoFV_onshell_physical.cpp \
		$(DIR)/MSSMNoFV_onshell_problems.cpp \
		$(DIR)/MSSMNoFV_onshell_soft_parameters.cpp \
		$(DIR)/MSSMNoFV_onshell_susy_parameters.cpp

# source files with main()
EXEsrc_SRC := \
		$(DIR)/gm2calc.cpp \
		$(DIR)/gm2scan.cpp

LIBsrc_OBJ := $(LIBsrc_SRC:.cpp=.o)

EXEsrc_OBJ := $(EXEsrc_SRC:.cpp=.o)

LIBsrc_DEP := $(LIBsrc_OBJ:.o=.d)

EXEsrc_DEP := $(EXEsrc_OBJ:.o=.d)

EXEsrc_EXE := $(patsubst $(DIR)/%.o, $(BINDIR)/%.x, $(EXEsrc_OBJ))

LIBsrc     := $(DIR)/lib$(MODNAME)$(LIBEXT)

clean::
	-rm -f $(LIBsrc_DEP) $(EXEsrc_DEP)
	-rm -f $(LIBsrc_OBJ) $(EXEsrc_OBJ)
	-rm -f $(LIBsrc)
	-rm -f $(EXEsrc_EXE)

$(LIBsrc_DEP) $(EXEsrc_DEP) $(LIBsrc_OBJ) $(EXEsrc_OBJ): \
	override CPPFLAGS += $(EIGENFLAGS) $(BOOSTFLAGS)

$(LIBsrc): $(LIBsrc_OBJ)
	$(MAKELIB) $@ $^

$(BINDIR)/%.x: $(DIR)/%.o $(LIBsrc) | $(BINDIR)
	$(CXX) -o $@ $^ $(LDLIBS)

ALLDEP += $(LIBsrc_DEP) $(EXEsrc_DEP)
ALLLIB += $(LIBsrc)
ALLEXE += $(EXEsrc_EXE)
