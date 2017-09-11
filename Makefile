OUTPUT = DemoManMonitor
CXX = g++
CXX_FLAGS = -std=c++14 -Wall -Werror -Wno-unused-result -O2
PROJECT_DIR = .
TESTS_DIR = $(PROJECT_DIR)/tests
GTEST_DIR = $(PROJECT_DIR)/gtest-1.7.0
MODELS_DIR = $(PROJECT_DIR)/models
SOURCES = DemoManMonitor.cpp AlsaSource.cpp AlsaSink.cpp PocketSphinxKWS.cpp Adafruit_Thermal.cpp
TEST_SOURCES = $(TESTS_DIR)/test_DemoManMonitor.cpp $(TESTS_DIR)/test_Adafruit_Thermal.cpp
#LIBS = -lwiringPi -lpocketsphinx -lsphinxbase -lsphinxad -lasound
LIBS = -lpocketsphinx -lsphinxbase -lsphinxad -lasound
INCLUDES = -I/usr/local/include/sphinxbase -I/usr/local/include/pocketsphinx
TEST_INCLUDES = -I$(PROJECT_DIR) -isystem $(GTEST_DIR)/include -I$(GTEST_DIR)


DemoManMonitor: $(SOURCES) main.cpp
	$(CXX) $(CXX_FLAGS) $(INCLUDES) $(SOURCES) main.cpp -o $(OUTPUT) $(LIBS)

debug: $(SOURCES) main.cpp
	$(CXX) -g $(CXX_FLAGS) $(INCLUDES) $(SOURCES) main.cpp -o $(OUTPUT) $(LIBS)

run: DemoManMonitor
	./DemoManMonitor -hmm ./models/en-us/ -dict ./models/cmudict-en-us.dict -lm ./models/en-us.lm.bin

runtests: tests
	./testrunner

tests: $(TEST_SOURCES) gtest-all.o gtest_main.o
	$(CXX) $(CXX_FLAGS) $(TEST_INCLUDES) $(INCLUDES) $(SOURCES) $(TEST_SOURCES) -pthread $(LIBS) gtest_main.o gtest-all.o -o testrunner

AlsaSourceRecorder: $(SOURCES)
	$(CXX) $(CXX_FLAGS) $(TEST_INCLUDES) $(INCLUDES) $(SOURCES) ./tests/AlsaSourceRecorder.cpp -o AlsaSourceRecorder $(LIBS)

AlsaSinkPlayer: $(SOURCES)
	$(CXX) $(CXX_FLAGS) $(TEST_INCLUDES) $(INCLUDES) $(SOURCES) ./tests/AlsaSinkPlayer.cpp -o AlsaSinkPlayer $(LIBS)

AdafruitThermalTest: $(SOURCES)
	$(CXX) $(CXX_FLAGS) $(TEST_INCLUDES) $(INCLUDES) $(SOURCES) ./tests/AdafruitThermalTest.cpp -o AdafruitThermalTest $(LIBS)

gtest-all.o:
	$(CXX) $(CXX_FLAGS) $(TEST_INCLUDES) -pthread -c $(GTEST_DIR)/src/gtest-all.cc

gtest_main.o:
	$(CXX) $(CXX_FLAGS) $(TEST_INCLUDES) -pthread -c $(GTEST_DIR)/src/gtest_main.cc

clean:
	rm -f *.o
	rm -f $(OUTPUT)
	rm -f testrunner
	rm -f AlsaSourceRecorder
	rm -f AlsaSinkPlayer
	rm -f AdafruitThermalTest
