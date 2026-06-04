
# Programming project

## Group elements

Identify all group elements (numbers and names).

- up202503835 João Gravanita
- up202503888 Bruno Alves
- up202504181 António Mascarenhas


## Accomplished tasks

Implementámos a leitura de ficheiros SVG, convertendo elementos XML em objetos C++ (como Rect, Circle, Polygon), organizados numa hierarquia de classes SVGElement que suporta grupos e a reutilização de componentes (Use).
Desenvolvemos uma estrutura de matrizes para gerir transformações afins. Configurámos a lógica para aplicar translações, rotações e escalas de forma precisa, utilizando a normalização em relação ao transform-origin para garantir que as transformações respeitem o ponto central de cada elemento.
Implementámos a lógica de desenho onde cada elemento calcula a sua matriz final através da pós-multiplicação, assegurando que a hierarquia do SVG (a relação pai-filho) seja respeitada corretamente.
Adicionámos uma função clamp em todas as operações de desenho para garantir que qualquer coordenada calculada fique estritamente dentro dos limites da imagem, evitando erros de memória e garantindo a correta renderização do canvas.


