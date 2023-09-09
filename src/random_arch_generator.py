import random as rand
import json

def dividir_lista(lista, k):
    sublistas = []

    for tamanho_sublista, quantidade in k.items():
        tamanho_sublista = int(tamanho_sublista)
        for _ in range(quantidade):
            sublista = lista[:tamanho_sublista]
            sublistas.append(sublista)
            lista = lista[tamanho_sublista:]

    return sublistas

def generate_architecture(filename, config, netsize, arcsize):

    # generate random labels
    input_label  = list(range(netsize))
    output_label = list(range(netsize))
    # rand.shuffle(input_label)
    # rand.shuffle(output_label)
    
    # divide labels into sets of wires
    input_label  = dividir_lista(input_label,  config['input'])
    output_label = dividir_lista(output_label, config['output'])

    _str=""
    pe_i = 0

    with open("misc/arch/"+filename, 'w') as file:

        _str += f"{{\n\t\"num_pes\":{arcsize},\n\n"
        
        for i in range(arcsize):

            _str += f"\t\"PE{i}\":{{\n"

            inputs  = ','.join(str(v) for v in input_label[pe_i])
            outputs = ','.join(str(v) for v in output_label[pe_i])

            _str += f"\t\t\"input\" :[{inputs}],\n"
            _str += f"\t\t\"output\":[{outputs}]\n"

            pe_i += 1
            
            if i == arcsize-1:
                _str += f"\t}}\n"
            else:
                _str += f"\t}},\n\n"

        _str += f"}}"

        file.write(_str)
        
def main():

    # set up config here
    with open('misc/architecture_config.json') as file:
        data = json.load(file)
        filename = data['filename']
        config   = data['config']
        netsize  = data['netsize']
        arcsize  = max( sum(config['input'].values()), sum(config['output'].values()) )
    
    # gen. random architecture
    generate_architecture(filename, config, netsize, arcsize)

if __name__ == '__main__':
    main()