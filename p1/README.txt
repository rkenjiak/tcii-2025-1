

link: https://youtu.be/3a6VxRwRI1U

Atividades realizadas:
1) A1: Finalização de KdTree<D,R,A>::Node.
2) A2: Finalização do construtor KdTree(A&& points, const Params& params); e métodos auxiliares para construção da kdtree.
3) A3: Finalização da classe KNN e sua respectiva busca de vizinhos pela quantidade k e pelo raio.
4) A4: Realização de 5 testes: 
    i) 2 testes com PointVector<D, R> (D igual a 2 e 3 e R igual a double e float) com e sem filtro (utilizou-se um filtro de distancia);
    ii) 2 testes com Sistema de Particulas, com e sem filtro (utilizou-se um filtro que ignorava cores vermelhas (1,0,0));
    iii) E um criando 100000 pontos e fez as buscas sem filtro.
    Observou-se a quantidade esperadas de pontos encontrados nos testes.
4) A5: Video gravado.

Arquivos dentro de p1/ com identificação do aluno no inicio são os manipulados para este problema. Arquivos em include/ foram retirados
do repositório como estãom exceto ParticleBuffer e ParticleSystem que foram retirados do ex3 com seus pequenos ajustes. Além de Vec2.h 
e Bounds2.h que foi praticamente igual aos respectivos em 3d.