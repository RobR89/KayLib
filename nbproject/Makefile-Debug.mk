#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux
CND_DLIB_EXT=so
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/DB/KMySQL.o \
	${OBJECTDIR}/DB/KSQL.o \
	${OBJECTDIR}/DB/KSQLite.o \
	${OBJECTDIR}/Graphics/KFont.o \
	${OBJECTDIR}/Graphics/KFontProperties.o \
	${OBJECTDIR}/Graphics/KImage.o \
	${OBJECTDIR}/IO/KFile.o \
	${OBJECTDIR}/Parser/JSON.o \
	${OBJECTDIR}/Parser/StringParser.o \
	${OBJECTDIR}/Parser/XMLDocument.o \
	${OBJECTDIR}/Scripting/KLUA.o \
	${OBJECTDIR}/String/KString.o \
	${OBJECTDIR}/String/KUTF.o \
	${OBJECTDIR}/Utility/DataCode.o \
	${OBJECTDIR}/Utility/KEventRate.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libkaylib.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libkaylib.a: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libkaylib.a
	${AR} -rv ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libkaylib.a ${OBJECTFILES} 
	$(RANLIB) ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libkaylib.a

${OBJECTDIR}/DB/KMySQL.o: DB/KMySQL.cpp
	${MKDIR} -p ${OBJECTDIR}/DB
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/DB/KMySQL.o DB/KMySQL.cpp

${OBJECTDIR}/DB/KSQL.o: DB/KSQL.cpp
	${MKDIR} -p ${OBJECTDIR}/DB
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/DB/KSQL.o DB/KSQL.cpp

${OBJECTDIR}/DB/KSQLite.o: DB/KSQLite.cpp
	${MKDIR} -p ${OBJECTDIR}/DB
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/DB/KSQLite.o DB/KSQLite.cpp

${OBJECTDIR}/Graphics/KFont.o: Graphics/KFont.cpp
	${MKDIR} -p ${OBJECTDIR}/Graphics
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Graphics/KFont.o Graphics/KFont.cpp

${OBJECTDIR}/Graphics/KFontProperties.o: Graphics/KFontProperties.cpp
	${MKDIR} -p ${OBJECTDIR}/Graphics
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Graphics/KFontProperties.o Graphics/KFontProperties.cpp

${OBJECTDIR}/Graphics/KImage.o: Graphics/KImage.cpp
	${MKDIR} -p ${OBJECTDIR}/Graphics
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Graphics/KImage.o Graphics/KImage.cpp

${OBJECTDIR}/IO/KFile.o: IO/KFile.cpp
	${MKDIR} -p ${OBJECTDIR}/IO
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/IO/KFile.o IO/KFile.cpp

${OBJECTDIR}/Parser/JSON.o: Parser/JSON.cpp
	${MKDIR} -p ${OBJECTDIR}/Parser
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Parser/JSON.o Parser/JSON.cpp

${OBJECTDIR}/Parser/StringParser.o: Parser/StringParser.cpp
	${MKDIR} -p ${OBJECTDIR}/Parser
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Parser/StringParser.o Parser/StringParser.cpp

${OBJECTDIR}/Parser/XMLDocument.o: Parser/XMLDocument.cpp
	${MKDIR} -p ${OBJECTDIR}/Parser
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Parser/XMLDocument.o Parser/XMLDocument.cpp

${OBJECTDIR}/Scripting/KLUA.o: Scripting/KLUA.cpp
	${MKDIR} -p ${OBJECTDIR}/Scripting
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Scripting/KLUA.o Scripting/KLUA.cpp

${OBJECTDIR}/String/KString.o: String/KString.cpp
	${MKDIR} -p ${OBJECTDIR}/String
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/String/KString.o String/KString.cpp

${OBJECTDIR}/String/KUTF.o: String/KUTF.cpp
	${MKDIR} -p ${OBJECTDIR}/String
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/String/KUTF.o String/KUTF.cpp

${OBJECTDIR}/Utility/DataCode.o: Utility/DataCode.cpp
	${MKDIR} -p ${OBJECTDIR}/Utility
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Utility/DataCode.o Utility/DataCode.cpp

${OBJECTDIR}/Utility/KEventRate.o: Utility/KEventRate.cpp
	${MKDIR} -p ${OBJECTDIR}/Utility
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Utility/KEventRate.o Utility/KEventRate.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
