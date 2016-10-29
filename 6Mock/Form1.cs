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
        private Pen pen;
        private Brush wBrush, bBrush;
        private int turn_cnt = 1;
        private Bitmap BlackStone = new Bitmap("Black.png");
        private Bitmap WhiteStone = new Bitmap("White.png");

        private Bitmap BlackStonePanel;
        private Bitmap WhiteStonePanel;

        private bool turn = false;// false : 흑돌 , true : 백돌

        enum STONE { none, Black, White, Block };
        STONE[,] 바둑판 = new STONE[19, 19];

        public Form1()
        {
            InitializeComponent();
            pen = new Pen(Color.Black);
            wBrush = new SolidBrush(Color.White);
            bBrush = new SolidBrush(Color.Black);
            panel1.Enabled = false;
            BlackStonePanel = new Bitmap(BlackStone, this.pictureBox1.Size);
            WhiteStonePanel = new Bitmap(WhiteStone, this.pictureBox1.Size);
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
            draw돌(x, y);
        }

        private void draw화점(Graphics g)
        {
            for(int x=3; x<=15; x+=6)
                for(int y = 3; y<=15; y+=6)
                {
                    Rectangle r = new Rectangle(10+ 눈size * x - 화점size / 2, 10 + 눈size * y - 화점size / 2, 화점size, 화점size);
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
            }
            else  // 흰돌
            {
                //g.FillEllipse(wBrush, r);
                g.DrawImage(WhiteStone, r);
                바둑판[x, y] = STONE.White;
            }
            turn_cnt++;
            this.label2.Text = (2 - turn_cnt).ToString();
            if (turn_cnt == 2)
            {
                turn = !turn;  // 돌 색깔을 토글
                if (!turn) this.pictureBox1.Image = BlackStonePanel;
                else this.pictureBox1.Image = WhiteStonePanel;
                turn_cnt = 0;
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
                if (y + i <= 18 && 바둑판[x, y+i] == 바둑판[x, y])
                    cnt++;
                else
                    break;
            for (int i = 1; i < 6; i++)
                if (y - i >= 0 && 바둑판[x, y-i] == 바둑판[x, y])
                    cnt++;
                else
                    break;
            return cnt;
        }

        private int checkSLASH(int x, int y)
        {
            int cnt = 1;
            for (int i = 1; i < 6; i++)
                if (x + i <= 18 && y-i >=0 && 바둑판[x + i, y-i] == 바둑판[x, y])
                    cnt++;
                else
                    break;
            for (int i = 1; i < 6; i++)
                if (x - i >= 0 && y+i <=18 && 바둑판[x - i, y+i] == 바둑판[x, y])
                    cnt++;
                else
                    break;
            return cnt;
        }

        private void 새게임NToolStripMenuItem_Click(object sender, EventArgs e)
        {
            panel1.Enabled = true;
            turn_cnt = 1;
            this.pictureBox1.Image = BlackStonePanel;
            for (int x = 0; x < 19; x++)
                for (int y = 0; y < 19; y++)
                    바둑판[x, y] = STONE.none;
            turn = false;
            Refresh();
            draw바둑판();
            this.label2.Text = (2 - turn_cnt).ToString();

            initBoard();
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
                if (x - i >= 0 && y - i >=0  && 바둑판[x - i, y - i] == 바둑판[x, y])
                    cnt++;
                else
                    break;
            return cnt;
        }
        [DllImport("6MockPlayer.dll")]
        extern public static void initBoard();
        [DllImport("6MockPlayer.dll")]
        extern public static void setBlocks(int[] x, int[] y, int cnt);
        [DllImport("6MockPlayer.dll")]
        extern public static void updateStones(int[] x, int[] y, int cnt);
        [DllImport("6MockPlayer.dll")]
        extern public static void playerTurn(int[] x, int[] y, int cnt);
    }
}
