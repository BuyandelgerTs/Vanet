#
# OMNeT++/OMNEST Makefile for vanet
#
# This file was generated with the command:
#  opp_makemake -f --deep -O out -I../mixim/src/base/connectionManager -I../mixim/src/base/messages -I../mixim/src/base/phyLayer -I../mixim/src/base/modules -I../mixim/src/base/utils -I../mixim/src/modules/obstacle -I../mixim/src/modules/messages -I../mixim/src/modules/analogueModel -I../mixim/src/modules/utility -I../mixim/src/modules/phy -I../mixim/src/modules/mac/ieee80211p -I../mixim/src/modules -L../mixim/out/$$\(CONFIGNAME\)/tests/testUtils -L../mixim/out/$$\(CONFIGNAME\)/src/base -L../mixim/out/$$\(CONFIGNAME\)/src/modules -lmiximtestUtils -lmiximbase -lmiximmodules -KMIXIM_PROJ=../mixim
#

# Name of target to be created (-o option)
TARGET = vanet$(EXE_SUFFIX)

# User interface (uncomment one) (-u option)
USERIF_LIBS = $(ALL_ENV_LIBS) # that is, $(TKENV_LIBS) $(CMDENV_LIBS)
#USERIF_LIBS = $(CMDENV_LIBS)
#USERIF_LIBS = $(TKENV_LIBS)

# C++ include paths (with -I)
INCLUDE_PATH = \
    -I../mixim/src/base/connectionManager \
    -I../mixim/src/base/messages \
    -I../mixim/src/base/phyLayer \
    -I../mixim/src/base/modules \
    -I../mixim/src/base/utils \
    -I../mixim/src/modules/obstacle \
    -I../mixim/src/modules/messages \
    -I../mixim/src/modules/analogueModel \
    -I../mixim/src/modules/utility \
    -I../mixim/src/modules/phy \
    -I../mixim/src/modules/mac/ieee80211p \
    -I../mixim/src/modules \
    -I. \
    -Iclasses \
    -Iclasses/mac \
    -Iclasses/nic \
    -Iclasses/phy \
    -Iresults

# Additional object and library files to link with
EXTRA_OBJS =

# Additional libraries (-L, -l options)
LIBS = -L../mixim/out/$(CONFIGNAME)/tests/testUtils -L../mixim/out/$(CONFIGNAME)/src/base -L../mixim/out/$(CONFIGNAME)/src/modules  -lmiximtestUtils -lmiximbase -lmiximmodules
LIBS += -Wl,-rpath,`abspath ../mixim/out/$(CONFIGNAME)/tests/testUtils` -Wl,-rpath,`abspath ../mixim/out/$(CONFIGNAME)/src/base` -Wl,-rpath,`abspath ../mixim/out/$(CONFIGNAME)/src/modules`

# Output directory
PROJECT_OUTPUT_DIR = out
PROJECTRELATIVE_PATH =
O = $(PROJECT_OUTPUT_DIR)/$(CONFIGNAME)/$(PROJECTRELATIVE_PATH)

# Object files for local .cc and .msg files
OBJS = $O/classes/mac/MyMac1609_4.o $O/classes/phy/MyPhyLayer80211p.o $O/EMSPkt_m.o

# Message files
MSGFILES = \
    EMSPkt.msg

# Other makefile variables (-K)
MIXIM_PROJ=../mixim

#------------------------------------------------------------------------------

# Pull in OMNeT++ configuration (Makefile.inc or configuser.vc)

ifneq ("$(OMNETPP_CONFIGFILE)","")
CONFIGFILE = $(OMNETPP_CONFIGFILE)
else
ifneq ("$(OMNETPP_ROOT)","")
CONFIGFILE = $(OMNETPP_ROOT)/Makefile.inc
else
CONFIGFILE = $(shell opp_configfilepath)
endif
endif

ifeq ("$(wildcard $(CONFIGFILE))","")
$(error Config file '$(CONFIGFILE)' does not exist -- add the OMNeT++ bin directory to the path so that opp_configfilepath can be found, or set the OMNETPP_CONFIGFILE variable to point to Makefile.inc)
endif

include $(CONFIGFILE)

# Simulation kernel and user interface libraries
OMNETPP_LIB_SUBDIR = $(OMNETPP_LIB_DIR)/$(TOOLCHAIN_NAME)
OMNETPP_LIBS = -L"$(OMNETPP_LIB_SUBDIR)" -L"$(OMNETPP_LIB_DIR)" -loppmain$D $(USERIF_LIBS) $(KERNEL_LIBS) $(SYS_LIBS)

COPTS = $(CFLAGS)  $(INCLUDE_PATH) -I$(OMNETPP_INCL_DIR)
MSGCOPTS = $(INCLUDE_PATH)

# we want to recompile everything if COPTS changes,
# so we store COPTS into $COPTS_FILE and have object
# files depend on it (except when "make depend" was called)
COPTS_FILE = $O/.last-copts
ifneq ($(MAKECMDGOALS),depend)
ifneq ("$(COPTS)","$(shell cat $(COPTS_FILE) 2>/dev/null || echo '')")
$(shell $(MKPATH) "$O" && echo "$(COPTS)" >$(COPTS_FILE))
endif
endif

#------------------------------------------------------------------------------
# User-supplied makefile fragment(s)
# >>>
# <<<
#------------------------------------------------------------------------------

# Main target
all: $O/$(TARGET)
	$(Q)$(LN) $O/$(TARGET) .

$O/$(TARGET): $(OBJS)  $(wildcard $(EXTRA_OBJS)) Makefile
	@$(MKPATH) $O
	@echo Creating executable: $@
	$(Q)$(CXX) $(LDFLAGS) -o $O/$(TARGET)  $(OBJS) $(EXTRA_OBJS) $(AS_NEEDED_OFF) $(WHOLE_ARCHIVE_ON) $(LIBS) $(WHOLE_ARCHIVE_OFF) $(OMNETPP_LIBS)

.PHONY: all clean cleanall depend msgheaders

.SUFFIXES: .cc

$O/%.o: %.cc $(COPTS_FILE)
	@$(MKPATH) $(dir $@)
	$(qecho) "$<"
	$(Q)$(CXX) -c $(CXXFLAGS) $(COPTS) -o $@ $<

%_m.cc %_m.h: %.msg
	$(qecho) MSGC: $<
	$(Q)$(MSGC) -s _m.cc $(MSGCOPTS) $?

msgheaders: $(MSGFILES:.msg=_m.h)

clean:
	$(qecho) Cleaning...
	$(Q)-rm -rf $O
	$(Q)-rm -f vanet vanet.exe libvanet.so libvanet.a libvanet.dll libvanet.dylib
	$(Q)-rm -f ./*_m.cc ./*_m.h
	$(Q)-rm -f classes/*_m.cc classes/*_m.h
	$(Q)-rm -f classes/mac/*_m.cc classes/mac/*_m.h
	$(Q)-rm -f classes/nic/*_m.cc classes/nic/*_m.h
	$(Q)-rm -f classes/phy/*_m.cc classes/phy/*_m.h
	$(Q)-rm -f results/*_m.cc results/*_m.h

cleanall: clean
	$(Q)-rm -rf $(PROJECT_OUTPUT_DIR)

depend:
	$(qecho) Creating dependencies...
	$(Q)$(MAKEDEPEND) $(INCLUDE_PATH) -f Makefile -P\$$O/ -- $(MSG_CC_FILES)  ./*.cc classes/*.cc classes/mac/*.cc classes/nic/*.cc classes/phy/*.cc results/*.cc

# DO NOT DELETE THIS LINE -- make depend depends on it.
$O/EMSPkt_m.o: EMSPkt_m.cc \
	$(MIXIM_PROJ)/src/base/messages/MacPkt_m.h \
	$(MIXIM_PROJ)/src/base/utils/MiXiMDefs.h \
	$(MIXIM_PROJ)/src/base/utils/SimpleAddress.h \
	$(MIXIM_PROJ)/src/base/utils/miximkerneldefs.h \
	EMSPkt_m.h
$O/classes/mac/MyMac1609_4.o: classes/mac/MyMac1609_4.cc \
	$(MIXIM_PROJ)/src/base/connectionManager/BaseConnectionManager.h \
	$(MIXIM_PROJ)/src/base/connectionManager/ChannelAccess.h \
	$(MIXIM_PROJ)/src/base/connectionManager/NicEntry.h \
	$(MIXIM_PROJ)/src/base/messages/AirFrame_m.h \
	$(MIXIM_PROJ)/src/base/messages/ChannelSenseRequest_m.h \
	$(MIXIM_PROJ)/src/base/messages/MacPkt_m.h \
	$(MIXIM_PROJ)/src/base/modules/BaseBattery.h \
	$(MIXIM_PROJ)/src/base/modules/BaseLayer.h \
	$(MIXIM_PROJ)/src/base/modules/BaseMacLayer.h \
	$(MIXIM_PROJ)/src/base/modules/BaseMobility.h \
	$(MIXIM_PROJ)/src/base/modules/BaseModule.h \
	$(MIXIM_PROJ)/src/base/modules/BaseWorldUtility.h \
	$(MIXIM_PROJ)/src/base/modules/BatteryAccess.h \
	$(MIXIM_PROJ)/src/base/phyLayer/AnalogueModel.h \
	$(MIXIM_PROJ)/src/base/phyLayer/BaseDecider.h \
	$(MIXIM_PROJ)/src/base/phyLayer/BasePhyLayer.h \
	$(MIXIM_PROJ)/src/base/phyLayer/ChannelInfo.h \
	$(MIXIM_PROJ)/src/base/phyLayer/ChannelState.h \
	$(MIXIM_PROJ)/src/base/phyLayer/Decider.h \
	$(MIXIM_PROJ)/src/base/phyLayer/DeciderToPhyInterface.h \
	$(MIXIM_PROJ)/src/base/phyLayer/Interpolation.h \
	$(MIXIM_PROJ)/src/base/phyLayer/MacToPhyControlInfo.h \
	$(MIXIM_PROJ)/src/base/phyLayer/MacToPhyInterface.h \
	$(MIXIM_PROJ)/src/base/phyLayer/Mapping.h \
	$(MIXIM_PROJ)/src/base/phyLayer/MappingBase.h \
	$(MIXIM_PROJ)/src/base/phyLayer/MappingUtils.h \
	$(MIXIM_PROJ)/src/base/phyLayer/PhyUtils.h \
	$(MIXIM_PROJ)/src/base/phyLayer/Signal_.h \
	$(MIXIM_PROJ)/src/base/utils/Coord.h \
	$(MIXIM_PROJ)/src/base/utils/FWMath.h \
	$(MIXIM_PROJ)/src/base/utils/FindModule.h \
	$(MIXIM_PROJ)/src/base/utils/HostState.h \
	$(MIXIM_PROJ)/src/base/utils/MiXiMDefs.h \
	$(MIXIM_PROJ)/src/base/utils/Move.h \
	$(MIXIM_PROJ)/src/base/utils/NetwToMacControlInfo.h \
	$(MIXIM_PROJ)/src/base/utils/PassedMessage.h \
	$(MIXIM_PROJ)/src/base/utils/SimpleAddress.h \
	$(MIXIM_PROJ)/src/base/utils/miximkerneldefs.h \
	$(MIXIM_PROJ)/src/modules/analogueModel/JakesFading.h \
	$(MIXIM_PROJ)/src/modules/analogueModel/LogNormalShadowing.h \
	$(MIXIM_PROJ)/src/modules/analogueModel/SimplePathlossModel.h \
	$(MIXIM_PROJ)/src/modules/mac/ieee80211p/Mac1609_4.h \
	$(MIXIM_PROJ)/src/modules/mac/ieee80211p/Mac80211pToPhy11pInterface.h \
	$(MIXIM_PROJ)/src/modules/mac/ieee80211p/WaveAppToMac1609_4Interface.h \
	$(MIXIM_PROJ)/src/modules/messages/Mac80211Pkt_m.h \
	$(MIXIM_PROJ)/src/modules/messages/WaveShortMessage_m.h \
	$(MIXIM_PROJ)/src/modules/phy/Decider80211p.h \
	$(MIXIM_PROJ)/src/modules/phy/Decider80211pToPhy80211pInterface.h \
	$(MIXIM_PROJ)/src/modules/phy/PhyLayer.h \
	$(MIXIM_PROJ)/src/modules/phy/PhyLayer80211p.h \
	$(MIXIM_PROJ)/src/modules/phy/SNRThresholdDecider.h \
	$(MIXIM_PROJ)/src/modules/utility/Consts80211p.h \
	classes/mac/MyMac1609_4.h
$O/classes/phy/MyPhyLayer80211p.o: classes/phy/MyPhyLayer80211p.cc \
	$(MIXIM_PROJ)/src/base/connectionManager/BaseConnectionManager.h \
	$(MIXIM_PROJ)/src/base/connectionManager/ChannelAccess.h \
	$(MIXIM_PROJ)/src/base/connectionManager/NicEntry.h \
	$(MIXIM_PROJ)/src/base/messages/AirFrame_m.h \
	$(MIXIM_PROJ)/src/base/messages/ChannelSenseRequest_m.h \
	$(MIXIM_PROJ)/src/base/modules/BaseBattery.h \
	$(MIXIM_PROJ)/src/base/modules/BaseMobility.h \
	$(MIXIM_PROJ)/src/base/modules/BaseModule.h \
	$(MIXIM_PROJ)/src/base/modules/BaseWorldUtility.h \
	$(MIXIM_PROJ)/src/base/modules/BatteryAccess.h \
	$(MIXIM_PROJ)/src/base/phyLayer/AnalogueModel.h \
	$(MIXIM_PROJ)/src/base/phyLayer/BaseDecider.h \
	$(MIXIM_PROJ)/src/base/phyLayer/BasePhyLayer.h \
	$(MIXIM_PROJ)/src/base/phyLayer/ChannelInfo.h \
	$(MIXIM_PROJ)/src/base/phyLayer/ChannelState.h \
	$(MIXIM_PROJ)/src/base/phyLayer/Decider.h \
	$(MIXIM_PROJ)/src/base/phyLayer/DeciderToPhyInterface.h \
	$(MIXIM_PROJ)/src/base/phyLayer/Interpolation.h \
	$(MIXIM_PROJ)/src/base/phyLayer/MacToPhyControlInfo.h \
	$(MIXIM_PROJ)/src/base/phyLayer/MacToPhyInterface.h \
	$(MIXIM_PROJ)/src/base/phyLayer/Mapping.h \
	$(MIXIM_PROJ)/src/base/phyLayer/MappingBase.h \
	$(MIXIM_PROJ)/src/base/phyLayer/MappingUtils.h \
	$(MIXIM_PROJ)/src/base/phyLayer/PhyUtils.h \
	$(MIXIM_PROJ)/src/base/phyLayer/Signal_.h \
	$(MIXIM_PROJ)/src/base/utils/Coord.h \
	$(MIXIM_PROJ)/src/base/utils/FWMath.h \
	$(MIXIM_PROJ)/src/base/utils/FindModule.h \
	$(MIXIM_PROJ)/src/base/utils/HostState.h \
	$(MIXIM_PROJ)/src/base/utils/MiXiMDefs.h \
	$(MIXIM_PROJ)/src/base/utils/Move.h \
	$(MIXIM_PROJ)/src/base/utils/miximkerneldefs.h \
	$(MIXIM_PROJ)/src/modules/analogueModel/BreakpointPathlossModel.h \
	$(MIXIM_PROJ)/src/modules/analogueModel/JakesFading.h \
	$(MIXIM_PROJ)/src/modules/analogueModel/LogNormalShadowing.h \
	$(MIXIM_PROJ)/src/modules/analogueModel/PERModel.h \
	$(MIXIM_PROJ)/src/modules/analogueModel/SimpleObstacleShadowing.h \
	$(MIXIM_PROJ)/src/modules/analogueModel/SimplePathlossModel.h \
	$(MIXIM_PROJ)/src/modules/analogueModel/TwoRayInterferenceModel.h \
	$(MIXIM_PROJ)/src/modules/mac/ieee80211p/Mac80211pToPhy11pInterface.h \
	$(MIXIM_PROJ)/src/modules/messages/AirFrame11p_m.h \
	$(MIXIM_PROJ)/src/modules/obstacle/Obstacle.h \
	$(MIXIM_PROJ)/src/modules/obstacle/ObstacleControl.h \
	$(MIXIM_PROJ)/src/modules/phy/Decider80211p.h \
	$(MIXIM_PROJ)/src/modules/phy/Decider80211pToPhy80211pInterface.h \
	$(MIXIM_PROJ)/src/modules/phy/PhyLayer.h \
	$(MIXIM_PROJ)/src/modules/phy/PhyLayer80211p.h \
	$(MIXIM_PROJ)/src/modules/phy/SNRThresholdDecider.h \
	$(MIXIM_PROJ)/src/modules/utility/Consts80211p.h \
	$(MIXIM_PROJ)/src/modules/world/annotations/AnnotationManager.h \
	classes/phy/MyPhyLayer80211p.h

