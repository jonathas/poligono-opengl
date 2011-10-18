#include <gl/glut.h>
#include <math.h>

const GLfloat tam_x = 50.0f;
const GLfloat tam_y = 50.0f;

GLfloat angulo = 0.0f;
GLfloat escala = 1.0f;
GLfloat dx = 0.0f;
GLfloat dy = 0.0f;

GLint largura_vp, altura_vp; //Largura e Altura da viewport para calcular as coordenadas do mouse
GLfloat xm, ym; //Pontos x e y do clique do mouse

//Arrays para guardar vértices dos segmentos de reta
GLfloat posx[100], posy[100];

//Flags
bool fill = false, posfound = false, theend = false, delpos = false;

// Função callback chamada para fazer o desenho
void desenha(void) {
	//Limpa a janela de visualização com a cor de fundo especificada 
	glClear(GL_COLOR_BUFFER_BIT);
    
    //Só pode adicionar ou deletar uma posição, se não tiver chegado ao fim ainda.
    if(theend == false) {
		posfound = false;
		if(delpos == true) {
			for(int i=0;i<=100 && posfound == false;i++) {
				if(posx[i] == 0 && posy[i] == 0) {
					//Se o i atual é vazio, o anterior não era. Vamos zerá-lo então:
					posx[i-1] = 0;
					posy[i-1] = 0;
					//Zerando também o penúltimo vértice, pra deletar o segmento que vai de um vértice a outro
					posx[i-2] = 0;
					posy[i-2] = 0;
					posfound = true;
				}
			}
			delpos = false;
		}
		//Percorre os arrays para adicionar próximo vértice
		for(int i=0;i<=100 && posfound == false;i++) {
			if(posx[i] == 0 && posy[i] == 0) {
				posx[i] = xm;
				posy[i] = ym;
				posfound = true; //Posição vazia encontrada e valores adicionados. Sai do For.
			}
		}
	}
	
	if(theend == false) {
		glColor3f(0.0f, 0.0f, 0.0f);
		glBegin(GL_LINE_STRIP);
	} else {
		glLoadIdentity();
		glTranslatef(dx, dy, 0.0f);
		glScalef(escala, escala, 0.0f);
		glTranslatef(0.0f, -20.0f, 0.0f);
		glRotatef(angulo, 0.0f, 0.0f, 1.0f);
		glTranslatef(0.0f, 20.0f, 0.0f);
		glColor3f(1.0f, 0.0f, 0.0f);
		glBegin(fill ? GL_POLYGON : GL_LINE_LOOP);
	}
	
	//Desenhando...
    for(int i=0;i<=100 && posx[i] != 0 && posy[i] != 0;i++) {	
		glVertex2i(posx[i], posy[i]);
	}
	glEnd();
	glLoadIdentity();
    
	//Executa os comandos OpenGL 
	glFlush();
    
    glLoadIdentity();	
}

void redimensiona(GLsizei largura, GLsizei altura) {
     // Redimensiona a viewport para ficar com o mesmo tamanho da janela
     glViewport(0, 0, largura, altura);    
     
     largura_vp = largura;
     altura_vp = altura; 
   
    // Inicializa o sistema de coordenadas
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    // Faz o mapeamento entre a viewport e o sistema de coordenadas 2D
    // levando em consideracao a relacao entre a largura e a altura da viewport
    // Nesse caso, o objeto renderizado vai mudar de tamanho conforme a janela
    // aumentar ou diminuir
    if (largura <= altura)
       gluOrtho2D(-tam_x, tam_x, -tam_y * altura / largura, tam_y * altura / largura);
    else
       gluOrtho2D(-tam_x * largura / altura, tam_x * largura / altura, -tam_y, tam_y);
       
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void teclado(unsigned char tecla, int x, int y) {
	switch(tecla) {
		case 's':
		case 'S':
			angulo += 5;
			break;
		case 'l':
		case 'L':
			angulo -= 5;
			break;
		case '+':
			escala += 0.2;
			break;
		case '-':
			escala -= 0.2;
			break;
		case ' ':
			fill = ! fill;
			break;
		case 'f':
		case 'F':
			theend = true;
			break;
		case 'd':
		case 'D':
			delpos = true;
			break;
		default:;
	}

	if (angulo >= 360 || angulo <= -360)
		angulo = 0;
	
	//Limitando o quanto o polígono pode ser diminuído ou aumentado
	if(escala < 0.2) {
		escala = 0.2;
	} else if(escala > 1.5) {
		escala = 1.5;
	}
	
	glutPostRedisplay();
}

void tecladoEspecial(int tecla, int x, int y) {
	switch(tecla) {
		case GLUT_KEY_UP:
			dy += 2;
			break;
		case GLUT_KEY_DOWN:
			dy -= 2;
			break;
		case GLUT_KEY_LEFT:
			dx -= 2;
			break;
		case GLUT_KEY_RIGHT:
			dx += 2;
			break;
		default:;
		break;
	}

     glutPostRedisplay();
}

void mouse(int botao, int estado, int x, int y) {
    if (botao == GLUT_LEFT_BUTTON)
		if (estado == GLUT_DOWN) {
			xm = ( (2 * tam_x * x) / largura_vp) - tam_x;
			ym = ( ( (2 * tam_y) * (y - altura_vp) ) / - altura_vp) - tam_y;
			angulo = 0;
		}

     glutPostRedisplay();
}

// Inicializa parâmetros de rendering
void inicializa (void) {
    // Define a cor de fundo da janela de visualização (RGB)
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(400,400);
    glutInitWindowPosition(10,10);
	glutCreateWindow("Polígono - Jonathas Rodrigues");
	glutDisplayFunc(desenha);
	glutSpecialFunc(tecladoEspecial);
	glutKeyboardFunc(teclado);
	glutMouseFunc(mouse);
	glutReshapeFunc(redimensiona);
	inicializa();
	glutMainLoop();
	return 0;
}
