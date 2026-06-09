CXX=g++
#CXXFLAGS= -std=c++17 -g -fprofile-arcs -ftest-coverage
CXXFLAGS= 

OATPP_SWAGGER_INCLUDE = /usr/local/include/oatpp-1.3.0/oatpp-swagger
OATPP_INCLUDE_LIB = /usr/local/include/oatpp-1.3.0/oatpp
OATPP_INCLUDE = -I src -I $(OATPP_INCLUDE_LIB) -I $(OATPP_SWAGGER_INCLUDE)

LINKFLAGS_LOCAL = -L /usr/local/lib/oatpp-1.3.0
LINKFLAGS_APP =  $(LINKFLAGS_LOCAL) -loatpp-swagger -loatpp
LINKFLAGS_TEST = $(LINKFLAGS_APP) -loatpp-test

SRC_DIR = src
SRC_DIR_SERVER = src/server
SRC_DIR_DTO = src/dto
SRC_DIR_CONTROLLER = src/controller
SRC_DIR_SERVICE_COMMENT = src/service/comment

SRC_DIR_TESTS = test
# SRC_DIR_TESTS_COMMENT = test/issue


GCOV = gcov
LCOV = lcov
COVERAGE_RESULTS = results.coverage
COVERAGE_DIR = coverage

STATIC_ANALYSIS = cppcheck

STYLE_CHECK = cpplint

PROGRAM_SERVER = commentServer
TEST_SERVER = testServer

.PHONY: all
all: $(PROGRAM_SERVER) coverage docs static style

# default rule for compiling .cc to .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean-gcov
clean-gov:
	rm -rf *.gcov *.gcda *.gcno

.PHONY: clean
clean:
	rm -rf *~ $(SRC)/*.o $(TEST_SRC)/*.o *.gcov *.gcda *.gcno \
	$(COVERAGE_RESULTS) \
	$(PROGRAM_SERVER) \
	$(TEST_SERVER) \
	$(COVERAGE_DIR) \
	doxygen/html \
	obj bin \

server: $(PROGRAM_SERVER)

$(PROGRAM_SERVER): $(SRC_DIR_SERVER) $(SRC_DIR_SERVICE)
	$(CXX) $(CXXFLAGS) -o $(PROGRAM_SERVER) $(OATPP_INCLUDE) \
	$(SRC_DIR_SERVER)/*.cpp \
	$(SRC_DIR_DTO)/*.hpp \
	$(SRC_DIR_CONTROLLER)/*.hpp \
	$(SRC_DIR_SERVICE_COMMENT)/*.hpp \
	$(SRC_DIR_SERVICE_COMMENT)/*.cpp \
	$(LINKFLAGS_APP)

$(TEST_SERVER): $(SRC_DIR_TESTS)
	$(CXX) $(CXXFLAGS) -o $(TEST_SERVER) $(OATPP_INCLUDE) \
	$(SRC_DIR_TESTS_COMMENT)/*.cpp \
	$(SRC_DIR_TESTS)/*.cpp $(SRC_DIR_SERVICE_COMMENT)/*.hpp $(LINKFLAGS_TEST)

docker:
	docker build --pull --rm -f "Dockerfile" -t shaperest:latest "."

start:
	docker compose -f "compose.yml" up -d --build

stop:
	docker compose -f "compose.yml" down

style: ${SRC_DIR_SERVICE} ${SRC_INCLUDE}
	${STYLE_CHECK} src/controller/* src/dto/* src/server/* src/service/*/* src/test/*/* src/test/*.cpp

# To perform the code coverage checks
.PHONY: coverage
coverage: ${TEST_SERVER}
	./${TEST_SERVER}
	# Determine code coverage
	${LCOV} --capture --gcov-tool ${GCOV} --directory . --output-file \
	${COVERAGE_RESULTS} --rc lcov_branch_coverage=1
	# Only show code coverage for the source code files (not library files)
	${LCOV} --extract ${COVERAGE_RESULTS} */${SRC_DIR}/* -o ${COVERAGE_RESULTS}
	#Generate the HTML reports
	genhtml ${COVERAGE_RESULTS} --output-directory ${COVERAGE_DIR}
	#Remove all of the generated files from gcov
	make clean-gcov
