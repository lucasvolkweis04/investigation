# 🔍 Análise Forense de Imagens: Detecção de Manipulações via Amplificação de Ruído

## 🎯 O Problema e o Contexto

Com o avanço das ferramentas de edição e a popularização de algoritmos de inteligência artificial (Deep Fakes, Generative AI), tornou-se um desafio complexo atestar a veracidade de uma fotografia. A utilização maliciosa dessas técnicas, especialmente na propagação de fake news, exige o desenvolvimento de métodos de verificação eficientes.

O objetivo deste trabalho prático é implementar um algoritmo em **linguagem C** capaz de realizar uma análise forense digital simplificada, mas altamente efetiva. A técnica baseia-se na **amplificação do ruído intrínseco da imagem**. Como edições e manipulações tendem a alterar o padrão de ruído original de uma foto (criando discrepâncias de compressão), nosso algoritmo destaca essas distorções, tornando visível o que foi alterado artificialmente.

---

## ⚙️ Arquitetura e Lógica do Algoritmo

O desenvolvimento do filtro foi estruturado em uma **pipeline de processamento de pixels**, operando sobre os canais **RGB**. Os passos implementados na função `processa()` foram:

### 1. Inicialização e Estrutura de Dados

As imagens de entrada (`in`) e saída (`out`) são manipuladas na memória como **arrays unidimensionais de objetos `Pixel`**. Cada pixel armazena individualmente seus componentes de cor:

- `r` (vermelho)
- `g` (verde)
- `b` (azul)

---

### 2. Definição da Janela de Varredura (Kernel)

O algoritmo utiliza uma **janela móvel de 3x3 pixels** (totalizando 9 pixels por iteração).

Foram inicializados arrays específicos para armazenar os valores de cor da vizinhança do pixel central:

```
r_vals
g_vals
b_vals
```

---

### 3. Tratamento de Bordas e Aplicação do Filtro

O código percorre iterativamente cada pixel `(x, y)` da imagem.

Para evitar **falhas de segmentação** ou erros ao acessar pixels nas extremidades da imagem, implementamos **verificações de limite**. Valores que "vazariam" da matriz são substituídos pelos valores da **borda mais próxima**, garantindo a integridade do processamento.

---

### 4. Ordenação (Bubble Sort) e Cálculo da Mediana

Após popular a janela **3x3**, é necessário encontrar a cor predominante (**mediana**) para isolar o ruído.

Como restrição do escopo, não pudemos utilizar funções externas como `qsort`. A solução foi implementar o algoritmo de **Bubble Sort** nativamente para ordenar os arrays de cores:

- R
- G
- B

A mediana é então extraída do **índice central do array ordenado**.

---

### 5. Amplificação do Ruído (Diferença)

O núcleo da análise forense ocorre aqui.

O algoritmo calcula a **diferença exata entre o valor RGB original do pixel central e a mediana de sua vizinhança**.

Essa diferença (o **ruído**) é multiplicada por um **fator de amplificação ajustável**.

Por fim, os resultados são **limitados (clamped)** ao intervalo válido de cores:

```
[0, 255]
```

Esses valores compõem a **imagem de saída**, onde possíveis manipulações tornam-se visíveis.

---

## 🚧 Desafios e Aprendizados

A transição da teoria para a implementação prática apresentou desafios significativos de lógica e sintaxe:

### Restrições de Escopo

A tentativa inicial de utilizar o `qsort` esbarrou na limitação de não podermos declarar funções auxiliares fora do escopo `void processa()`. Isso exigiu a refatoração completa da lógica de ordenação para um **Bubble Sort interno**.

### Depuração Visual

Diferente de dados textuais, o **debug de imagens é complexo**. Enfrentamos diversas iterações onde o processamento matemático gerava outputs indesejados:

- imagens totalmente pretas
- artefatos de cor distorcidos

Isso exigiu revisão minuciosa dos cálculos de **limite de matriz e RGB**.

### Melhorias Futuras

O código atual atende aos requisitos, mas há espaço para otimização.

A **complexidade ciclomática do Bubble Sort para cada pixel** aumenta o custo computacional. Como trabalho futuro, a lógica poderia ser:

- simplificada
- vetorizada
- paralelizada

Isso permitiria processar **imagens em resoluções maiores** de forma mais performática.

---

## 👨🏻‍💻 Autoria

Desenvolvido por **Leonardo Chiao** e **Lucas Volkweis** para o **Trabalho Prático 1** da disciplina de **Investigação Digital (Outubro, 2024)**.
