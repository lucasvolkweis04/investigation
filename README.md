Aluno: Leonardo Chiao & Lucas Volkweis 
Relatório de atividade -
Trabalho Prático 1 - Investigação Digital
Outubro, 2024

 1. Introdução: apresentação do problema:
Atualmente tem sido cada vez mais difícil confirmar em imagens: há uma grande quantidade de técnicas de manipulação de imagens, desde edição manual até técnicas envolvendo algoritmos de aprendizagem de máquina. A utilização maliciosa dessas técnicas, especialmente para a produção de fake news, tem sido cada vez mais frequente.
A ideia deste trabalho é explorar um algoritmo capaz de realizar uma análise forense simplificada de imagens, porém bastante efetiva em algumas situações. Esse algoritmo consiste em amplificar o ruído já presente na imagem, de forma que seja possível enxergar distorções na qualidade desse ruído e identificar possíveis manipulações.

 2. Desenvolvimento: conjunto de passos a serem realizados e validação de cada passo, descrição do(s) algoritmo(s)
1. Inicialização da Imagem de Saída:
As imagens de entrada (in) e saída (out) são representadas como arrays unidimensionais de objetos Pixel, onde cada pixel tem componentes r (vermelho), g (verde) e b (azul).
2. Definição e Inicialização de Variáveis para o Filtro:
Agora, o código define o tamanho da janela como 3x3 pixels e inicializa arrays (r_vals, g_vals, b_vals) para armazenar os valores de cor dessa janela. O número de pixels é fixado em 9 (3x3).
  
 3. Aplicação do Filtro de Mediana
O código então percorre cada pixel da imagem (x, y), preenchendo a janela 3x3 com os valores dos pixels ao redor (incluindo o próprio pixel central). Para garantir que o código lide com pixels nas bordas da imagem, ele utiliza verificações que limitam os índices da janela, substituindo valores fora dos limites pelos valores da borda mais próxima, conforme instrução nos slides do professor.
 
 4. Ordenação dos Valores para Encontrar a Mediana
Após preencher a janela com os valores RGB, o código usa um Bubble Sort para ordenar os valores dos três canais de cor (R, G, B).
Ao final, o valor mediano de cada canal é encontrado no meio do array ordenado.
  
 5. Amplificação da Diferença Entre o Pixel Central e a Mediana
Em seguida, o código calcula a diferença entre os valores RGB do pixel central da janela e a mediana dos valores da janela. Essa diferença é amplificada por um fator ajustável (fator). Após a amplificação, os valores são limitados ao intervalo [0, 255], garantindo que estejam dentro do intervalo válido para cores.
3. Conclusão: principais dificuldades encontradas e possíveis melhorias
Tivemos bastante dificuldade para aplicar nossas ideias, e achar alguma maneira de começar o código. Até chegar nessa versão final, tivemos vários protótipos (incluindo um com qsort), porém ele necessitava a declaração da função fora do void processa(), o que não era permitido. Perguntamos inúmeras vezes para o professor em aula, e aos nossos colegas em busca de ideias. Múltiplas vezes a imagem de saída não era o que queríamos(às vezes ficava tudo preto, às vezes um emaranhado de cores) ou o programa simplesmente não executava. Como possíveis melhorias, nosso código poderia ter ficado mais simples.
 