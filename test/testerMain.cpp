#include <sstream>
#include "manifest.hpp"
using namespace CPP_TESTER;
/* make a new out stream */
ostringstream __std_buffer;
/* make a backup from current stdout */
streambuf* const __stdout = std::cout.rdbuf();
/* global accessable current testcase */
test_case* __test_case = NULL;
/* init test indexer/counter */
size_t __testCounter = 0;
/**
 * at exit handler
 */
void atExit(void);
/**
 * flush buffer outputs to origin stdout
 */
void flush_buffer(void);
/**
 * Clearouts stdbuffer
 */
void clear_buffer(void);
/**
 * Redirect the stdout
 * @param change2buffer if it is true then the stdout will be redirected to
 * &nbsp;&nbsp;buffer, otherwise it will restored to origin stdout stream line.
 */
void change_stdout(bool change2buffer = false);
/*
 * The main entery point of test project
 */
int main(int, char**) {
    /* register an atExit handler */
    atexit(&atExit);
    /* bootstrap the test suites */
    __bootstrap();
    /* fetch test suite */
    test_suite __test_suite = registery::getTestSuites();
    /* foreach test case */
    for(; __testCounter< __test_suite.size(); __testCounter++)
    {
        /* fetch a test case */
        __test_case = &__test_suite.at(__testCounter);
        /* Promt the test-case */
        std::cout<<"[.] "<<__test_case->first<<std::flush;
        /* change the stdout to buffer std stream */
        change_stdout(true);
        {
            /* pre-run the test case */
            __test_case->second.first->pre_run();
            /* run the test case */
            __test_case->second.first->run(__test_case->second.second.size(), __test_case->second.second.data());
            /* post-run the test case */
            __test_case->second.first->post_run();
            /* delete the test case's instance */
            delete __test_case->second.first;
        }
        /* restore the stdout to it origin out streamer */
        change_stdout(false);
        /* flush the testcase's stdout buffer */
        flush_buffer();
    }
    /* we do not want re-print the last testcase status */
    __test_case = NULL;
    /* successful termination*/
    TERMINATE_SUCCESS;
}
void atExit(void) {
    /* change to origin stdout streamer */
    change_stdout(false);
    /* flush any unflushed buffer stream */
    flush_buffer();
    /* status report */
    echo(std::endl<<"Total `"<<__testCounter<<"` have been carried out!");
    /* exiting */
    echo("[ TERMINATING ]");
}
void flush_buffer(void) {
    /* change to origin stdout */
    change_stdout(false);
    /* catch the output */
    std::string __output = __std_buffer.str();
    /* output las testcase status */
    if(__test_case != NULL)
        /* output the check status */
        echo("\r[\u221A] "<< __test_case->first);
    /* if there was any output */
    if(__output.length() != 0)
    {
        std::string __headline = string(20, '-') + (" Output ") + string(21, '-');
        std::string __endline = string(17, '-') + (" End Of Output ") + string(17, '-');
        echo(__headline << std::endl <<__output << std::endl << __endline);
    }
    /* clearout the buffer */
    clear_buffer();
}
void clear_buffer(void) {
    /* clear out the buffer */
    __std_buffer.clear();
    __std_buffer.str(std::string(""));
}
void change_stdout(bool change2buffer) {
    if(change2buffer) {
        /* clear out the buffer */
        clear_buffer();
        /* replace the new out stream with stdout */
        cout.rdbuf( __std_buffer.rdbuf() );
    } else {
        /* restore old cout. */
        cout.rdbuf( __stdout );
    }
}
