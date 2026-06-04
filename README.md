
# Programming project

## Group elements

Identify all group elements (numbers and names).

- up202503835 João Gravanita
- up202503888 Bruno Alves
- up202504181 António Mascarenhas


## Accomplished tasks

Neste projeto, começámos por implementar a leitura de ficheiros SVG, convertendo os elementos descritos em XML em objetos em C++, como retângulos, círculos e polígonos. Estes elementos foram organizados numa estrutura hierárquica baseada numa classe geral, o que nos permitiu representar também grupos de elementos e reutilizar componentes através do mecanismo de “use”.

De seguida, desenvolvemos uma forma de representar transformações geométricas através de matrizes. Isto permitiu-nos aplicar alterações como deslocamentos, rotações e escalas aos elementos de forma consistente. Tivemos especial atenção ao ponto de origem das transformações, de modo a garantir que cada elemento fosse transformado corretamente em relação ao seu centro ou ponto definido.

Na fase de desenho, cada elemento calcula a sua posição final tendo em conta a sua matriz de transformação e a hierarquia em que está inserido, ou seja, a relação entre elementos “pai” e “filho” no SVG.

Por fim, adicionámos uma verificação para garantir que todas as coordenadas geradas se mantêm dentro dos limites do ecrã. Isto ajudou a evitar erros durante o desenho e contribuiu para uma renderização mais estável.
