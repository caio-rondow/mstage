#include "../include/omega.h"

/* Constructor/Destructor */
Omega::Omega(int netsize, int st, int ex, int radix){

    _netsize = netsize;
    _st      = st;
    _ex      = ex;
    _exsize  = pow(radix,ex);
    _mask    = netsize-1;

    _switch = new int*[netsize];
    _route_matrix   = new int*[netsize];
    for(int i=0; i<_netsize; i++){
        _switch[i] = new int[_st+_ex];
        _route_matrix[i]   = new int[_st+ex];
    }

	conn = vector<queue<int>>(NUM_PES);

    clear();
}

Omega::~Omega(){ destroy(); }

/* Aux. Functions */
void Omega::create(){
    _switch = nullptr;
    _route_matrix   = nullptr;
    _netsize      = 0;
    _st           = 0;
    _ex           = 0;
    _exsize       = 0;
    _mask         = 0;
}

void Omega::destroy(){

    for(int i=0; i<_netsize; i++){
        delete[] _switch[i];
        delete[] _route_matrix[i];
    }
    delete[] _switch;
    delete[] _route_matrix;

    _netsize = 0;
    _st      = 0;
    _ex      = 0;
    _exsize  = 0;
    _mask    = 0;
}

int Omega::window(int word, int col) const{ return ( word >> ( 2*(_st+_ex)- 2*(col+1) ) ) & _mask; }

int Omega::switch_code(int word, int col) const{ return ( word >> ( 2*(2*_st+_ex-1) - 2*col ) ) & 3; }

int Omega::concat(int input, int extra, int output) const{ return output | (input<<(2*_st+2*_ex)) | (extra<<(2*_st)); }

void Omega::unroute(int word){
    for(int j=0; j<_st+_ex; j++){
        int i = window(word,j);
        _route_matrix[i][j]  -= 1;
        if(_route_matrix[i][j] == 0)
            _switch[i][j] = 0;
    }
}

/* Methods */
bool Omega::route(int input, int output, int receiver_pe, int sender_pe){

    for(int extra=0; extra<_exsize; extra++){

        // supondo que encontrei um caminho
        bool res=true;

        // concatena palavra: entrada+extra+saida
        int word = concat(input, extra, output);

        // para todos os estagios...
        for(int j=0; j<_st+_ex; j++){
            
            // pega o valor da janela atual
            int i = window(word, j);

            // se o caminho esta ocupado
            // senão, permute um bit extra
            if(_route_matrix[i][j]) 
                res = res && ( _switch[i][j] == switch_code(word,j) ); 
        }

        // se encontrou um caminho...
        if(res){
            for(int j=0; j<_st+_ex; j++){

                int i = window(word,j);

                // marca o caminho que fez em circuit
                _route_matrix[i][j]   += 1;
                _switch[i][j] = switch_code(word,j);
            } 

            /* marca na tabela qual caminho eu fiz para input/output */
            conn[receiver_pe].push(word);
            conn[sender_pe].push(word);
		
            return true;
        }
    }

    return false;
}

int Omega::dealloc(const Architecture&arc, int pe){
    int num_dealloc_conn = 0;
    while(!conn[pe].empty()){
        /* remove from queue */
        int word = conn[pe].front();
        conn[pe].pop();
        /* undo connection */
        unroute(word);
        num_dealloc_conn++;
    }
    return num_dealloc_conn;
}

void Omega::clear(){ // reseta a rede
    for(int i=0; i<_netsize; i++){
        for(int j=0; j<_st+_ex; j++){
            _switch[i][j] = 0;
            _route_matrix[i][j]   = 0;
        }
    }
}

void Omega::display() const{
    cout << "\t";
    for(int i=0; i<_st+_ex; i++){
        cout << i << " ";
    } cout << "\n\n";

    for(int i=0; i<_netsize; i++){
        cout << i << "\t";
        for(int j=0; j<_st+_ex; j++){
            cout << _route_matrix[i][j] << " ";
        } cout << "\n";
    } 
}