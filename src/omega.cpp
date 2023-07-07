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

    clear();
}

Omega::~Omega(){ 
    destroy(); 
}

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

void Omega::desconcat(int word, int& input, int& extra, int& output) const{
    input = (word>>(2*_st + 2*_ex))&_mask;
    extra = (word>>(2*_st)&3);
    output = (word&_mask);
}

bool Omega::unroute(int word){

    if(word>=0){

        for(int j=0; j<_st+_ex; j++){
            int i = window(word,j);
            if(_route_matrix[i][j] == 1){
                _route_matrix[i][j]--;
                _switch[i][j] = -1;
            } else{
                _route_matrix[i][j]--;
            }     
        }
        
        /* fala que desroteou */
        return true;
    }

    return false;
}

/* Methods */
int Omega::route(int input, int output){

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

                // marca o caminho que fez
                _route_matrix[i][j]++;
                _switch[i][j] = switch_code(word,j);
            } 

            return word;
        }
    }

    return FAIL;
}

void Omega::clear(){ // reseta a rede
    for(int i=0; i<_netsize; i++){
        for(int j=0; j<_st+_ex; j++){
            _switch[i][j] = -1;
            _route_matrix[i][j] = 0;
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

void Omega::display_switch() const{

    cout << "\t";
    for(int i=0; i<_st+_ex; i++){
        cout << i << " ";
    } cout << "\n\n";

    for(int i=0; i<_netsize; i++){
        cout << i << "\t";
        for(int j=0; j<_st+_ex; j++){
            cout << _switch[i][j] << " ";
        } cout << "\n";
    } 

}

vector<vector<int>> Omega::get_net(){
    vector<vector<int>> copy_net(_netsize, vector<int>(_st+_ex, 0));
    for(int i=0; i<_netsize; i++){
        for(int j=0; j<_st+_ex; j++){
            copy_net[i][j] = _route_matrix[i][j];
        }
    }
    return copy_net;
}