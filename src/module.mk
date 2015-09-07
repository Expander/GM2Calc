DIR          := src
MODNAME      := gm2calc

# source files
LIBsrc_SRC := \
		$(DIR)/dilog.cpp \
		$(DIR)/ffunctions.cpp \
		$(DIR)/gm2_1loop.cpp \
		$(DIR)/gm2_2loop.cpp \
		$(DIR)/gm2_slha_io.cpp \
		$(DIR)/MSSMNoFV_onshell.cpp \
		$(DIR)/MSSMNoFV_onshell_mass_eigenstates.cpp \
		$(DIR)/MSSMNoFV_onshell_physical.cpp \
		$(DIR)/MSSMNoFV_onshell_problems.cpp \
		$(DIR)/MSSMNoFV_onshell_soft_parameters.cpp \
		$(DIR)/MSSMNoFV_onshell_susy_parameters.cpp

# main()
EXEsrc_SRC := \
		$(DIR)/gm2calc.cpp \
		$(DIR)/gm2scan.cpp

LIBsrc_OBJ := \
		$(patsubst %.cpp, %.o, $(filter %.cpp, $(LIBsrc_SRC)))

EXEsrc_OBJ := \
		$(EXEsrc_SRC:.cpp=.o)

LIBsrc_DEP := \
		$(LIBsrc_OBJ:.o=.d)

EXEsrc_DEP := \
		$(EXEsrc_OBJ:.o=.d)

EXEsrc_EXE := \
		$(patsubst $(DIR)/%.o, $(BINDIR)/%.x, $(EXEsrc_OBJ))

LIBsrc     := \
		$(DIR)/lib$(MODNAME)$(LIBEXT)

clean::
		-rm -f $(LIBsrc_DEP)
		-rm -f $(EXEsrc_DEP)
		-rm -f $(LIBsrc_OBJ)
		-rm -f $(EXEsrc_OBJ)
		-rm -f $(LIBsrc)
		-rm -f $(EXEsrc_EXE)

$(LIBsrc_DEP) $(EXEsrc_DEP) $(LIBsrc_OBJ) $(EXEsrc_OBJ): \
		override CPPFLAGS += $(EIGENFLAGS) $(BOOSTFLAGS)

$(LIBsrc): $(LIBsrc_OBJ)
		$(MAKELIB) $@ $^

$(BINDIR)/%.x: $(DIR)/%.o $(LIBsrc) $(LIBFLEXI) | $(BINDIR)
		$(CXX) -o $@ $^ $(LAPACKLIBS) $(BLASLIBS) $(FLIBS)

ALLDEP += $(LIBsrc_DEP) $(EXEsrc_DEP)
ALLLIB += $(LIBsrc)
ALLEXE += $(EXEsrc_EXE)
