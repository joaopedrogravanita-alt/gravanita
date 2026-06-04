
# Programming project

## Group elements

Identify all group elements (numbers and names).

- up202503835 João Gravanita
- up202503888 Bruno Alves
- up202504181 António Mascarenhas


## Accomplished tasks

1. Implementámos a leitura de ficheiros SVG, convertendo elementos XML em objetos C++ (como Rect, Circle, Polygon), organizados numa hierarquia de classes SVGElement que suporta grupos e a reutilização de componentes (Use).
2. Desenvolvemos uma estrutura de matrizes para gerir transformações afins. Configurámos a lógica para aplicar translações, rotações e escalas de forma precisa, utilizando a normalização em relação ao transform-origin para garantir que as transformações respeitem o ponto central de cada elemento.
3. Implementámos a lógica de desenho onde cada elemento calcula a sua matriz final através da pós-multiplicação, assegurando que a hierarquia do SVG (a relação pai-filho) seja respeitada corretamente.
4. Adicionámos uma função clamp em todas as operações de desenho para garantir que qualquer coordenada calculada fique estritamente dentro dos limites da imagem, evitando erros de memória e garantindo a correta renderização do canvas.

Neste projeto, começámos por implementar a leitura de ficheiros SVG, convertendo os elementos descritos em XML em objetos em C++, como retângulos, círculos e polígonos. Estes elementos foram organizados numa estrutura hierárquica baseada numa classe geral, o que nos permitiu representar também grupos de elementos e reutilizar componentes através do mecanismo de “use”.

De seguida, desenvolvemos uma forma de representar transformações geométricas através de matrizes. Isto permitiu-nos aplicar alterações como deslocamentos, rotações e escalas aos elementos de forma consistente. Tivemos especial atenção ao ponto de origem das transformações, de modo a garantir que cada elemento fosse transformado corretamente em relação ao seu centro ou ponto definido.

Na fase de desenho, cada elemento calcula a sua posição final tendo em conta a sua matriz de transformação e a hierarquia em que está inserido, ou seja, a relação entre elementos “pai” e “filho” no SVG.

Por fim, adicionámos uma verificação para garantir que todas as coordenadas geradas se mantêm dentro dos limites do ecrã. Isto ajudou a evitar erros durante o desenho e contribuiu para uma renderização mais estável.
