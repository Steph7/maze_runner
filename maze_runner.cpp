#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <thread>
#include <chrono>
#include <string>
#include <sstream>


// Representação do labirinto
using Maze = std::vector<std::vector<char>>;

// Estrutura para representar uma posição no labirinto
struct Position {
    int row;
    int col;
};

// Variáveis globais
Maze maze;
int num_rows;
int num_cols;
std::stack<Position> valid_positions;

// Função para imprimir o labirinto
void print_maze() {
    // TODO: Implemente esta função
    // 1. Percorra a matriz 'maze' usando um loop aninhado
    // 2. Imprima cada caractere usando std::cout
    // 3. Adicione uma quebra de linha (std::cout << '\n') ao final de cada linha do labirinto
    for(int i = 0; i < num_rows; i++){
        for(int j = 0; j < num_cols; j++){
            std::cout << maze[i][j];
        }
        std::cout << std::endl;
    }
}

// Função para carregar o labirinto de um arquivo
Position load_maze(const std::string& file_name) {
    // TODO: Implemente esta função seguindo estes passos:
    // 1. Abra o arquivo especificado por file_name usando std::ifstream
    // 2. Leia o número de linhas e colunas do labirinto
    // 3. Redimensione a matriz 'maze' de acordo (use maze.resize())
    // 4. Leia o conteúdo do labirinto do arquivo, caractere por caractere
    // 5. Encontre e retorne a posição inicial ('e')
    // 6. Trate possíveis erros (arquivo não encontrado, formato inválido, etc.)
    // 7. Feche o arquivo após a leituravalid format file 
    
    std::ifstream arquivo;
    arquivo.open(file_name);
    if (!arquivo) std::cerr << "Não foi possível ler arquivo: " << file_name << std::endl;

    // TRATAMENTO DE ERROS
    if (arquivo.fail())
    {
        // Código de Erro
        std::ios_base::iostate state = arquivo.rdstate();

        // Conferir tipos específicos de erro
        if (state & std::ios_base::eofbit)
        {
            std::cout << "End of file reached." << std::endl;
        }
        if (state & std::ios_base::failbit)
        {
            std::cout << "Non-fatal I/O error occurred." << std::endl;
        }
        if (state & std::ios_base::badbit)
        {
            std::cout << "Fatal I/O error occurred." << std::endl;
        }

        // Imprimir mensagem de erro
        std::perror("Erro: ");

        return {-1,-1};
    }

    std::string primeira_linha;
    if (std::getline(arquivo, primeira_linha)) {
        std::istringstream linha1(primeira_linha);
        std::vector<int> listaNumeros;
        int numero;

        while (linha1 >> numero) {
            listaNumeros.push_back(numero);
        }

        num_rows = listaNumeros[0];
        num_cols = listaNumeros[1];

        //std::cout << "LINHAS: " << num_rows << std::endl;
        //std::cout << "COLUNAS: " << num_cols << std::endl;

    } else {
        std::cerr << "Arquivo corrompido ou erro durante leitura." << std::endl;
    }

    std::string linha;
    while(getline(arquivo, linha)){
        std::vector<char> vetorLinha;
        vetorLinha.assign(linha.begin(), linha.end());

        maze.push_back(vetorLinha);
    } 

    // LER DADOS DO LABIRINTO (já colocados no vetor maze)
    //print_maze();

    // PROCURAR PELA ENTRADA DO LABIRINTO
    Position posicao_inicial;
    for(int i = 0; i < num_rows; i++){
        for(int j = 0; j < num_cols; j++){
            if(maze[i][j] == 'e'){
                posicao_inicial.row = i;
                posicao_inicial.col = j;
                valid_positions.push(posicao_inicial);

                // conferir posição inicial
                //std::cout << "R: " << posicao_inicial.row << std::endl;
                //std::cout << "C: " << posicao_inicial.col << std::endl;
            }
        }
    }

    arquivo.close();    
    return {posicao_inicial.row, posicao_inicial.col};
}

// Função para verificar se uma posição é válida
bool is_valid_position(int row, int col) {
    // TODO: Implemente esta função
    // 1. Verifique se a posição está dentro dos limites do labirinto
    //    (row >= 0 && row < num_rows && col >= 0 && col < num_cols)
    // 2. Verifique se a posição é um caminho válido (maze[row][col] == 'x')
    // 3. Retorne true se ambas as condições forem verdadeiras, false caso contrário
    if(row >= 0 && row < num_rows && col >= 0 && col < num_cols){
        if(maze[row][col] == 'x'){
            return true;
        }
        else if(maze[row][col] == 's'){
            return true;
        }
        else {
            return false;
        }
    }
    else {
        return false;
    }
}

// Função principal para navegar pelo labirinto
bool walk(Position pos) {
    // TODO: Implemente a lógica de navegação aqui
    // 1. Marque a posição atual como visitada (maze[pos.row][pos.col] = '.')
    // 2. Chame print_maze() para mostrar o estado atual do labirinto
    // 3. Adicione um pequeno atraso para visualização:
    //    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    // 4. Verifique se a posição atual é a saída (maze[pos.row][pos.col] == 's')
    //    Se for, retorne true
    // 5. Verifique as posições adjacentes (cima, baixo, esquerda, direita)
    //    Para cada posição adjacente:
    //    a. Se for uma posição válida (use is_valid_position()), adicione-a à pilha valid_positions
    // 6. Enquanto houver posições válidas na pilha (!valid_positions.empty()):
    //    a. Remova a próxima posição da pilha (valid_positions.top() e valid_positions.pop())
    //    b. Chame walk recursivamente para esta posição
    //    c. Se walk retornar true, propague o retorno (retorne true)
    // 7. Se todas as posições foram exploradas sem encontrar a saída, retorne false


    // Conferir se posição é a saída
    if(maze[pos.row][pos.col] == 's'){
        return true; // Encontrou saída!!!
    }
    else{
        maze[pos.row][pos.col] = '.';
        print_maze();
        
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        
        Position novaPosicao;
        // Cima
        novaPosicao.row = pos.row + 1;
        novaPosicao.col = pos.col;
        if(is_valid_position(novaPosicao.row, novaPosicao.col) == true){
            valid_positions.push(novaPosicao);
        } 
        // Baixo
        novaPosicao.row = pos.row - 1;
        novaPosicao.col = pos.col;
        if(is_valid_position(novaPosicao.row, novaPosicao.col) == true){
            valid_positions.push(novaPosicao);
        }
        // Esquerda 
        novaPosicao.row = pos.row;
        novaPosicao.col = pos.col - 1;
        if(is_valid_position(novaPosicao.row, novaPosicao.col) == true){
            valid_positions.push(novaPosicao);
        }
        // Direita
        novaPosicao.row = pos.row;
        novaPosicao.col = pos.col + 1;
        if(is_valid_position(novaPosicao.row, novaPosicao.col) == true){
            valid_positions.push(novaPosicao);
        }

        while(!valid_positions.empty()){
            Position posicaoAtual;
            posicaoAtual = valid_positions.top();
            valid_positions.pop();
            if(walk(posicaoAtual) == true){
                return true;
            }
            else{
                return false;
            }
        }
        return false;
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Uso: " << argv[0] << " <arquivo_labirinto>" << std::endl;
        return 1;
    }
    
    Position initial_pos = load_maze(argv[1]);
    if (initial_pos.row == -1 || initial_pos.col == -1) {
        std::cerr << "Posição inicial não encontrada no labirinto." << std::endl;
        return 1;
    }

    bool exit_found = walk(initial_pos);

    if (exit_found) {
        std::cout << "Saída encontrada!" << std::endl;
    } else {
        std::cout << "Não foi possível encontrar a saída." << std::endl;
    }

    return 0;
}

// Nota sobre o uso de std::this_thread::sleep_for:
// 
// A função std::this_thread::sleep_for é parte da biblioteca <thread> do C++11 e posteriores.
// Ela permite que você pause a execução do thread atual por um período especificado.
// 
// Para usar std::this_thread::sleep_for, você precisa:
// 1. Incluir as bibliotecas <thread> e <chrono>
// 2. Usar o namespace std::chrono para as unidades de tempo
// 
// Exemplo de uso:
// std::this_thread::sleep_for(std::chrono::milliseconds(50));
// 
// Isso pausará a execução por 50 milissegundos.
// 
// Você pode ajustar o tempo de pausa conforme necessário para uma melhor visualização
// do processo de exploração do labirinto.
