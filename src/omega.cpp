#include "../include/omega.h"

/* Constructor/Destructor */
Omega::Omega(int netsize, int st, int ex, int radix){

    _netsize = netsize;
    _st      = st;
    _ex      = ex;
    _exsize  = pow(radix,ex);
    _mask    = netsize-1;

    _route_matrix = new int*[netsize];
    _row_status   = new int*[netsize];
    for(int i=0; i<_netsize; i++){
        _route_matrix[i] = new int[_st+_ex];
        _row_status[i]   = new int[_st+ex];
    }

    _out_predecessor = vector<int>(_netsize,-1);
    _in_neighbor     = vector<vector<int>>(_netsize);

    clear();
}

Omega::~Omega(){

    _netsize = 0;
    _st      = 0;
    _ex      = 0;
    _exsize  = 0;
    _mask    = 0;

    for(int i=0; i<_netsize; i++){
        delete[] _route_matrix[i];
        delete[] _row_status[i];
    }
    delete[] _route_matrix;
    delete[] _row_status;
}

/* Aux. Functions */
int Omega::window(int word, int col) const{ return ( word >> ( 2*(_st+_ex)- 2*(col+1) ) ) & _mask; }

int Omega::switch_code(int word, int col) const{ return ( word >> ( 2*(2*_st+_ex-1) - 2*col ) ) & 3; }

int Omega::concat(int input, int extra, int output) const{ return output | (input<<(2*_st+2*_ex)) | (extra<<(2*_st)); }

void Omega::unroute(int word){

    for(int j=0; j<_st+_ex; j++){
        int i = window(word,j);
        _row_status[i][j]  -= 1;
        if(_row_status[i][j] == 0)
            _route_matrix[i][j] = 0;
    }
}

/* Methods */
bool Omega::route(int input, int output){

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
            if(_row_status[i][j]) 
                res = res && ( _route_matrix[i][j] == switch_code(word,j) ); 
        }

        // se encontrou um caminho...
        if(res){
            for(int j=0; j<_st+_ex; j++){

                int i = window(word,j);

                // marca o caminho que fez em circuit
                _row_status[i][j]   += 1;
                _route_matrix[i][j] = switch_code(word,j);
            } 

            /* marca na tabela qual caminho eu fiz para input/output */
            _in_neighbor[input].push_back(word);
            _out_predecessor[output] = word;

            return true;
        }
    }

    return false;
}

void Omega::dealloc(const Architecture&arc, int pe){
    /* get PE input/output */
    vector<int> pe_input  = arc.iport(pe);
    vector<int> pe_output = arc.oport(pe);

    /* iterate over the pe input/output and unroute them */
    for(auto &input:pe_input){
        int word = _out_predecessor[input];
        /* check if there is a valid word */
        if(word >= 0){
            unroute(word);
            _out_predecessor[input] = -1;
        }
    }
    for(auto &output:pe_output){
        for(auto &word:_in_neighbor[output]){
            unroute(word);
        }
        _in_neighbor[output].erase(_in_neighbor[output].begin(), _in_neighbor[output].end());
    }
}


void Omega::clear(){ // reseta a rede
    for(int i=0; i<_netsize; i++){
        for(int j=0; j<_st+_ex; j++){
            _route_matrix[i][j] = 0;
            _row_status[i][j]   = 0;
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
            cout << _row_status[i][j] << " ";
        } cout << "\n";
    } 
}