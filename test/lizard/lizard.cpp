#include "../manifest.hpp"

// init test indexer/counter
size_t lizard_test_counter = 0;
// make a new out stream
ostringstream lizard_std_buffer;
// make a backup from current stdout
streambuf* const lizard_stdout = std::cout.rdbuf();
// global accessable current testcase
test_case* lizard_test_case = NULL;
/**
 * @brief at exit handler
 */
void lizard_atExit(void);
/**
 * @brief flush buffer outputs to origin stdout
 */
void lizard_flush_buffer(void);
/**
 * @brief clearouts stdbuffer
 */
void lizard_clear_buffer(void);
/**
 * @brief redirect the stdout
 * @param change2buffer if it is true then the stdout will be redirected to
 *  buffer, otherwise it will restored to origin stdout stream line.
 */
void lizard_change_stdout(bool change2buffer = false);
/**
 * @brief the main entery point of test project
 */
int main(int, char**) {
    atexit(&::lizard_atExit);
    lizard_manifest();
    extern size_t lizard_test_counter;
    extern test_case* lizard_test_case;
    extern vector<test_case> lizard_registery;
    for(; lizard_test_counter < lizard_registery.size(); lizard_test_counter++)
    {
        lizard_test_case = &lizard_registery.at(lizard_test_counter);
        std::cout<<"[.] "<<lizard_test_case->first<<flush;
        lizard_change_stdout(true);
        {
            lizard_test_case->second->pre_run();
            lizard_test_case->second->run();
            lizard_test_case->second->post_run();
            delete lizard_test_case->second;
        }
        lizard_change_stdout(false);
        lizard_flush_buffer();
    }
    lizard_test_case = NULL;
    return EXIT_SUCCESS;
}

void lizard_atExit(void) {
    extern size_t lizard_test_counter;
    lizard_change_stdout(false);
    lizard_flush_buffer();
    cout<<std::endl<<"Total `"<<lizard_test_counter<<"` tests have been carried out!"<<endl<<flush;
    cout<<"[ TERMINATING ]"<<endl<<flush;
}

void lizard_flush_buffer(void) {
    extern test_case* lizard_test_case;
    extern ostringstream lizard_std_buffer;
    lizard_change_stdout(false);
    std::string __output = lizard_std_buffer.str();
    if(lizard_test_case != NULL)
        cout<<"\r[\u221A] "<< lizard_test_case->first<<endl<<flush;
    if(__output.length() != 0)
    {
        std::string __headline = string(20, '-') + (" Output ") + string(21, '-');
        std::string __endline = string(17, '-') + (" End Of Output ") + string(17, '-');
        cout<<__headline << std::endl <<__output << std::endl << __endline<<endl<<flush;
    }
    lizard_clear_buffer();
}

void lizard_clear_buffer(void) {
    extern ostringstream lizard_std_buffer;
    lizard_std_buffer.clear();
    lizard_std_buffer.str(std::string(""));
}

void lizard_change_stdout(bool change2buffer) {
    extern ostringstream lizard_std_buffer;
    if(change2buffer) {
        lizard_clear_buffer();
        cout.rdbuf( lizard_std_buffer.rdbuf() );
    } else {
        cout.rdbuf( lizard_stdout );
    }
}
