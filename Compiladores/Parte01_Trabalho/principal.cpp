#include "scanner.h"
#include <iomanip> // Para organizar em colunas

int main(int argc, char* argv[]) 
{
    if (argc != 2)
    {
        cout << "Uso: ./compiler nome_arquivo.mj\n";
        return 1;
    }

    Scanner* scanner = new Scanner(argv[1]);
    Token* t;
    
    cout << "\n─── INICIO DA ANALISE LEXICA ───\n";
    cout << left << setw(20) << "TIPO" << " │ " << "LEXEMA" << endl;
    cout << "───────────────────────────────────────" << endl;

    do
    {
        t = scanner->nextToken();
        
        // Exibe o nome amigável e o conteúdo do lexema
        cout << left << setw(20) << t->nameToString() 
             << " │ [" << t->lexeme << "]" 
             << endl;
             
    } while (t->name != END_OF_FILE);

    cout << "───────────────────────────────────────" << endl;
    cout << "Fim da analise.\n";

    delete scanner;
    return 0;
}