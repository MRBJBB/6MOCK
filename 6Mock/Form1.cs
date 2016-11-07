using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace _6Mock
{
    public partial class Form1 : Form
    {
        private int 돌size = 24;
        private int 눈size = 27;
        private int 화점size = 11;
        private int 막점size = 6;
        private Pen pen;
        private Brush wBrush, bBrush, rBrush;
        private int turn_cnt = 1;
        private Bitmap BlackStone = new Bitmap("Black.png");
        private Bitmap WhiteStone = new Bitmap("White.png");
        private Bitmap TrapStone = new Bitmap("Trap.png");

        private Bitmap BlackStonePanel;
        private Bitmap WhiteStonePanel;

        private bool turn = false;// false : 흑돌 , true : 백돌

        //        private int[,] latestWhite = new int[2, 2];
        //       private int[,] latestBlack = new int[2, 2];
        private int[,] latest = new int[2,2];


        enum STONE { none, Black, White, Block };
        STONE[,] 바둑판 = new STONE[19, 19];

        private int[] 블록킹x = new int[7];
        private int[] 블록킹y = new int[7];
        private int 블록킹수;
        private int 선;
        private int[] AIx = new int[2];
        private int[] AIy = new int[2];
        private int[] HumanX = new int[2];
        private int[] HumanY = new int[2];
        private int turn_num;

        public Form1()
        {
            InitializeComponent();
            pen = new Pen(Color.Black);
            wBrush = new SolidBrush(Color.White);
            bBrush = new SolidBrush(Color.Black);
            rBrush = new SolidBrush(Color.Red);
            panel1.Enabled = false;
            BlackStonePanel = new Bitmap(BlackStone, this.pictureBox1.Size);
            WhiteStonePanel = new Bitmap(WhiteStone, this.pictureBox1.Size);

            for(int i =0; i<2; i++)
            {
                for(int j =0; j<2; j++)
                {
                    //        latestBlack[i, j] = -1;
                    //        latestWhite[i, j] = -1;
                    latest[i, j] = -1;
                }
            }
        }

        private void 랜덤생성()
        {
            Random rand = new Random();
            선 = rand.Next(2);
            블록킹수 = rand.Next(4) * 2; // 0~ 6, even;

            if (선 == 1) MessageBox.Show("AI First");
            else MessageBox.Show("You First");

            for (int i = 0; i < 블록킹수; i++)
            {
                블록킹x[i] = rand.Next(19);
                블록킹y[i] = rand.Next(19);

                for (int j = 0; j < i; j++)
                {
                    if (블록킹x[i] == 블록킹x[j] && 블록킹y[i] == 블록킹y[j])
                    {
                        i--;
                        break;
                    }
                }
            }

            Graphics g = panel1.CreateGraphics();
            for (int i = 0; i < 블록킹수; i++)
            {
                Rectangle r = new Rectangle(10 + 눈size * 블록킹x[i] - 돌size / 2,
                     10 + 눈size * 블록킹y[i] - 돌size / 2, 돌size+5, 돌size+5);
                g.DrawImage(TrapStone, r);
                바둑판[블록킹x[i], 블록킹y[i]] = STONE.Block;
            }
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            draw바둑판();
        }

        private void draw바둑판()
        {
            Graphics g = panel1.CreateGraphics();
            for (int x = 0; x < 19; x++)
                g.DrawLine(pen, 10, 10 + x * 눈size, 495, 10 + x * 눈size);
            for (int y = 0; y < 19; y++)
                g.DrawLine(pen, 10 + y * 눈size, 10, 10 + y * 눈size, 10 + 18 * 눈size);
            draw화점(g);
        }

        private void panel1_MouseDown(object sender, MouseEventArgs e)
        {
            if (e.Button != MouseButtons.Left)
                return;
            int x, y;
            x = e.X / 눈size;
            y = e.Y / 눈size;
            if (x < 0 || x >= 19 || y < 0 || y >= 19)
                return;

            for(int i=0; i<블록킹수; i++)
            {
                if (x == 블록킹x[i] && y == 블록킹y[i]) return;
            }

            draw돌(x, y);
        }

        private void draw화점(Graphics g)
        {
            for (int x = 3; x <= 15; x += 6)
                for (int y = 3; y <= 15; y += 6)
                {
                    Rectangle r = new Rectangle(10 + 눈size * x - 화점size / 2, 10 + 눈size * y - 화점size / 2, 화점size, 화점size);
                    g.FillEllipse(bBrush, r);
                }
        }

        private void draw돌(int x, int y)
        {
            Graphics g = panel1.CreateGraphics();

            // 이미 어떤 돌이 놓여져 있으면 return
            if (바둑판[x, y] == STONE.Black || 바둑판[x, y] == STONE.White)
                return;

            Rectangle r = new Rectangle(10 + 눈size * x - 돌size / 2,
                 10 + 눈size * y - 돌size / 2, 돌size, 돌size);

            if (turn == false)    // 검은 돌
            {
                //g.FillEllipse(bBrush, r);
                g.DrawImage(BlackStone, r);
                바둑판[x, y] = STONE.Black;
                if (선 == 1)
                {
                    AIx[turn_cnt] = x;
                    AIy[turn_cnt] = y;
                }
                else
                {
                    HumanX[turn_cnt] = x;
                    HumanY[turn_cnt] = y;
                }
            }
            else  // 흰돌
            {
                //g.FillEllipse(wBrush, r);
                g.DrawImage(WhiteStone, r);
                바둑판[x, y] = STONE.White;
                if (선 == 0)
                {
                    AIx[turn_cnt] = x;
                    AIy[turn_cnt] = y;
                }
                else
                {
                    HumanX[turn_cnt] = x;
                    HumanY[turn_cnt] = y;
                }
            }
            turn_cnt++;
            this.label2.Text = (2 - turn_cnt).ToString();

            if(turn_cnt == 1)
            {
                if (turn)
                {
                    for (int i = 0; i < 2; i++)
                    {
                        if (latest[i, 0] == -1 || latest[i, 1] == -1) continue;
                        r = new Rectangle(10 + 눈size * latest[i, 0] - 돌size / 2,
                 10 + 눈size * latest[i, 1] - 돌size / 2, 돌size, 돌size);
                        g.DrawImage(BlackStone, r);
                    }

                }
                else
                {
                    for (int i = 0; i < 2; i++)
                    {
                        if (latest[i, 0] == -1 || latest[i, 1] == -1) continue;
                        r = new Rectangle(10 + 눈size * latest[i, 0] - 돌size / 2,
                 10 + 눈size * latest[i, 1] - 돌size / 2, 돌size, 돌size);
                        g.DrawImage(WhiteStone, r);
                    }
                }
            }
            r = new Rectangle(10 + 눈size * x - 막점size / 2, 10 + 눈size * y - 막점size / 2, 막점size, 막점size);

            g.FillEllipse(rBrush, r);
            latest[turn_cnt-1, 0] = x;
            latest[turn_cnt-1, 1] = y;

            if (turn_cnt == 2)
            {
                turn = !turn;  // 돌 색깔을 토글
                if (!turn) this.pictureBox1.Image = BlackStonePanel;
                else this.pictureBox1.Image = WhiteStonePanel;
                turn_cnt = 0;

                if (turn_num != 1)
                {
                    if ((선 == 1 && turn_num % 2 == 0) || (선 == 0 && turn_num % 2 == 1))
                    {
                        updateStones(HumanX, HumanY, 2);
                        turn_num++;
                        playerTurn(AIx, AIy, 2);
                        draw돌(AIx[0], AIy[0]);
                        draw돌(AIx[1], AIy[1]);
                    }
                    else
                    {
                        turn_num++;
                    }

                }
                else if (turn_num == 1 && 선 == 0)
                {
                    int[] startX = new int[2];
                    startX[0] = HumanX[1];
                    int[] startY = new int[2];
                    startY[0] = HumanY[1];
                    updateStones(startX, startY, 1);
                    turn_num++;
                    playerTurn(AIx, AIy, 2);
                    draw돌(AIx[0], AIy[0]);
                    draw돌(AIx[1], AIy[1]);
                    //turn_num++;
                }
                else if (turn_num == 1 && 선 == 1)
                {
                    turn_num++;
                }
            }
            

            check6mok(x, y);  // 육목이 만들어졌는지 체크하는 함수
        }

        private void check6mok(int x, int y)
        {
            if (checkLR(x, y) == 6)
            {
                MessageBox.Show(바둑판[x, y] + " wins");
                this.panel1.Enabled = false;
            }
            if (checkUD(x, y) == 6)
            {
                MessageBox.Show(바둑판[x, y] + " wins");
                this.panel1.Enabled = false;
            }
            if (checkSLASH(x, y) == 6)
            {
                MessageBox.Show(바둑판[x, y] + " wins");
                this.panel1.Enabled = false;
            }
            if (checkBACKSLASH(x, y) == 6)
            {
                MessageBox.Show(바둑판[x, y] + " wins");
                this.panel1.Enabled = false;
            }
        }

        private int checkLR(int x, int y)
        {
            int cnt = 1;
            for (int i = 1; i < 6; i++)
                if (x + i <= 18 && 바둑판[x + i, y] == 바둑판[x, y])
                    cnt++;
                else
                    break;
            for (int i = 1; i < 6; i++)
                if (x - i >= 0 && 바둑판[x - i, y] == 바둑판[x, y])
                    cnt++;
                else
                    break;
            return cnt;
        }

        private int checkUD(int x, int y)
        {
            int cnt = 1;
            for (int i = 1; i < 6; i++)
                if (y + i <= 18 && 바둑판[x, y + i] == 바둑판[x, y])
                    cnt++;
                else
                    break;
            for (int i = 1; i < 6; i++)
                if (y - i >= 0 && 바둑판[x, y - i] == 바둑판[x, y])
                    cnt++;
                else
                    break;
            return cnt;
        }

        private int checkSLASH(int x, int y)
        {
            int cnt = 1;
            for (int i = 1; i < 6; i++)
                if (x + i <= 18 && y - i >= 0 && 바둑판[x + i, y - i] == 바둑판[x, y])
                    cnt++;
                else
                    break;
            for (int i = 1; i < 6; i++)
                if (x - i >= 0 && y + i <= 18 && 바둑판[x - i, y + i] == 바둑판[x, y])
                    cnt++;
                else
                    break;
            return cnt;
        }

        private void 새게임NToolStripMenuItem_Click(object sender, EventArgs e)
        {
            panel1.Enabled = true;
            turn_cnt = 1;
            turn_num = 1;
            this.pictureBox1.Image = BlackStonePanel;
            for (int x = 0; x < 19; x++)
                for (int y = 0; y < 19; y++)
                    바둑판[x, y] = STONE.none;
            turn = false;
            Refresh();
            draw바둑판();
            this.label2.Text = (2 - turn_cnt).ToString();

            initBoard();
            랜덤생성();
            setBlocks(블록킹x, 블록킹y, 블록킹수);
            if (선 == 1)
            {
                playerTurn(AIx, AIy, 1);
                draw돌(AIx[0], AIy[0]);
                
                //playerTurn()
            }
            /*    else
                {
                    bool thisTurn = turn;
                    //while (thisTurn == turn) ;
                    int[] startX = new int[2];
                    startX[0] = HumanX[1];
                    int[] startY = new int[2];
                    startY[0] = HumanY[1];
                    updateStones(startX, startY, 1);

                    playerTurn(AIx, AIy, 2);
                    draw돌(AIx[0], AIy[0]);
                    draw돌(AIx[1], AIy[1]);
                }*/
        }

        private void 종료XToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Close();
        }

        private int checkBACKSLASH(int x, int y)
        {
            int cnt = 1;
            for (int i = 1; i < 6; i++)
                if (x + i <= 18 && y + i <= 18 && 바둑판[x + i, y + i] == 바둑판[x, y])
                    cnt++;
                else
                    break;
            for (int i = 1; i < 6; i++)
                if (x - i >= 0 && y - i >= 0 && 바둑판[x - i, y - i] == 바둑판[x, y])
                    cnt++;
                else
                    break;
            return cnt;
        }

        [DllImport("6MockPlayer.dll", CallingConvention = CallingConvention.Cdecl)]
        extern public static void initBoard();
        [DllImport("6MockPlayer.dll", CallingConvention = CallingConvention.Cdecl)]
        extern public static void setBlocks(int[] x, int[] y, int cnt);
        [DllImport("6MockPlayer.dll", CallingConvention = CallingConvention.Cdecl)]
        extern public static void updateStones(int[] x, int[] y, int cnt);
        [DllImport("6MockPlayer.dll", CallingConvention = CallingConvention.Cdecl)]
        extern public static void playerTurn(int[] x, int[] y, int cnt);
    }
}
