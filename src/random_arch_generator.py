import random as rand
import json

def split_tuples(lst, s2, s3):
    result = []

    j = 0
    for _ in range(0,s2*2,2):
        elem = (lst[j], lst[j+1])
        result.append(tuple(elem))
        j+=2

    k = j
    for _ in range(k, s3*3 + j, 3):
        elem = (lst[j], lst[j+1], lst[j+2])
        result.append(tuple(elem))
        j+=3
    
    k = j

    for _ in range(k, len(lst)):
        elem = (lst[j],)
        result.append(elem)
        j+=1

    return result

def generate_architecture(filename, config, netsize, arcsize):

    # get configuration
    input  = list(config["input"].values())
    output = list(config["output"].values())  

    # generate random labels
    input_label  = list(range(netsize))
    output_label = list(range(netsize))
    rand.shuffle(input_label)
    rand.shuffle(output_label)
    
    # divide labels into sets of wires
    input_label  = split_tuples(input_label, input[1], input[2])
    output_label = split_tuples(output_label, output[1], output[2])
    rand.shuffle(input_label)
    rand.shuffle(output_label)

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