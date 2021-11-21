#ifndef HELPERS_H_   
#define HELPERS_H_ 

#define RUNNING_DIR	"./" 
#define LOG_FILE	RUNNING_DIR"pokemon.log"

// Define the colours
#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

// Define pokemon capture status
enum Capture {PokemonEscaped=2, PokemonCaught=3, AshEscaped=4 };

int getRandom(int max);
void logger(const char* tag, const char* message);

#endif 