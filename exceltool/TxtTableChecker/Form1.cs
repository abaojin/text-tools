using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;

namespace TxtTableChecker
{
    public partial class Form1 : Form
    {
        private string defaultPath="";

        public Form1()
        {
            this.SizeChanged += new EventHandler(OnSizeChanged);
            InitializeComponent();

            RB_DE.Checked = true;
            //listBox1.Dock = DockStyle.Bottom;

            ReadDefaultPath();
        }

        private void OnSizeChanged( object sender , EventArgs args )
        {
            int w = this.Size.Width;
            int h = this.Size.Height - 80;

            listBox1.Left = 5;
            listBox1.Width = w - 15;
            listBox1.Top = 80 ;
            listBox1.Height = h - 20;
        }

        private void ReadDefaultPath()
        {
            if (!File.Exists("checkerCfg"))
                return;

            StreamReader sr = new StreamReader("checkerCfg");

            string str = sr.ReadLine();
            sr.Close(); 
            if ( str != null && str.Length > 0)
                defaultPath = str;

            textBox1.Text = defaultPath;
        }

        private  void WriteDefaultPath()
        {
            StreamWriter sw = new StreamWriter("checkerCfg");
            if( sw != null )
            {
                sw.WriteLine(defaultPath);
                sw.Close();
            }
        }

        private void Browse_Click(object sender, EventArgs e)
        {
            FolderBrowserDialog dlg = new FolderBrowserDialog();
            DialogResult dr = dlg.ShowDialog();
            if (dr != DialogResult.OK)
                return;

            if (dlg.SelectedPath.Length < 4)
                return;

            textBox1.Text = dlg.SelectedPath;
            defaultPath = textBox1.Text;
        }

        private  int CheckFile( string path )
        {
            int errCnt = 0;

            AddLog( "正在检查 " + path , false );

            List<string> lines = new List<string>();
            StreamReader sr = File.OpenText(path);

            while( !sr.EndOfStream )
                lines.Add(sr.ReadLine());

            sr.Close();

            //
            if( lines.Count < 4 )
            {
                MessageBox.Show( path + " 中没有值");
                return 1 ;
            }

            //
            int nameCnt = 0;
            int typeCnt = 0;
            int cmtCnt = 0;

            string[] names;
            string[] types;
            string[] cmts;

            if( RB_TK.Checked )
            {
                // name
                names = lines[1].Split(new char[] { '\t' }, StringSplitOptions.None);
                nameCnt = names.Length;

                AddLog("名字行， 列数是 : " + nameCnt.ToString(), false);

                // type
                types = lines[0].Split(new char[] { '\t' }, StringSplitOptions.None);
                typeCnt = types.Length;

                AddLog("类型行， 列数是 : " + typeCnt.ToString(), false);

                // commet
                cmts = lines[2].Split(new char[] { '\t' }, StringSplitOptions.None);
                cmtCnt = cmts.Length;

                AddLog("注释行， 列数是 : " + cmtCnt.ToString(), false);
            }
            else
            {
                // name
                names = lines[0].Split(new char[] { '\t' }, StringSplitOptions.None);
                nameCnt = names.Length;

                AddLog("名字行， 列数是 : " + nameCnt.ToString(), false);

                // type
                types = lines[1].Split(new char[] { '\t' }, StringSplitOptions.None);
                typeCnt = types.Length;

                AddLog("类型行， 列数是 : " + typeCnt.ToString(), false);

                // commet
                cmts = lines[2].Split(new char[] { '\t' }, StringSplitOptions.None);
                cmtCnt = cmts.Length;

                AddLog("注释行， 列数是 : " + cmtCnt.ToString(), false);
            }

            // check type string
            int tpCt = types.Length;
            for (int i = 0; i < tpCt; i ++ )
            {
                if (GetTypeStr(types[i]) == "Unknown Type")
                {
                    AddLog(path + " 的名类型名字有错误 ，第 "+ (i+1).ToString() + " 列 ：" + types[i] , true);

                    return 1;
                }
            }

            //
                if (nameCnt < 1 || nameCnt != typeCnt)
                {
                    AddLog(path + " 的名字数、类型数有错误或不一致", true);
                    return 1;
                }

            //
            if( nameCnt - cmts.Length > 0 )
            {
                string[] newCmts = new string[nameCnt];

                for( int i = 0 ; i < nameCnt ; i ++ )
                {
                    if (i < cmtCnt)
                        newCmts[i] = cmts[i];
                    else
                        newCmts[i] = "Default commet";
                }
            }

            //
            for( int i = 3 ; i < lines.Count ; i ++ )
            {
                string[] items = lines[i].Split(new char[] { '\t' }, StringSplitOptions.None);

                // column count .
                if( items.Length < nameCnt )
                {
                    AddLog(path + " 的第 " + (i + 1).ToString() + " 行 , 值太少 : " + items.Length.ToString() );
                    errCnt++;
                }
                else if (items.Length > nameCnt)
                {
                    AddLog(path + " 的第 " + (i + 1).ToString() + " 行 , 值太多 : " + items.Length.ToString());
                    errCnt++;
                }

                // type check.
                int c = items.Length;
                for( int kk = 0 ; kk < c ; kk ++ )
                {
                    if( items[kk].Length < 1 )
                    {
                        /*
                        if( RB_DE.Checked )
                        {
                            AddLog( path + " 的第 " + ( i + 1 ).ToString() + " 行 , 第 " + ( kk + 1 ).ToString() + " 列为空。") ;
                            errCnt++;
                        }
                        */
                    }
                    else
                    {
                        types[kk] = types[kk].ToLower();

                        string typeStr = GetTypeStr(types[kk]) ;

                        if ( typeStr  == "string")
                        {
                        }
                        else if (typeStr == "int" || typeStr == "uint" || typeStr == "byte" || typeStr == "ubyte")
                        {
                            if (!IsInt(items[kk]))
                            {
                                AddLog(path + " 的第 " + (i + 1).ToString() + " 行 , 第 " + (kk + 1).ToString() + " 列值类型不匹配，不是整数。" + items[kk] );
                                errCnt++;
                            }
                        }
                        else if (typeStr == "float" || typeStr == "double")
                        {
                            if( !IsFloat(items[kk]))
                            {
                                AddLog(path + " 的第 " + (i + 1).ToString() + " 行 , 第 " + (kk + 1).ToString() + " 列值类型不匹配，不是浮点数。" + items[kk]);
                                errCnt++;
                            }
                        }
                        else if (typeStr == "bool")
                        {
                            items[kk] = items[kk].ToLower();
                            if (items[kk] != "true" && items[kk] != "false" && items[kk] != "1" && items[kk] != "0" )
                            {
                                AddLog(path + " 的第 " + (i + 1).ToString() + " 行 , 第 " + (kk + 1).ToString() + " 列值类型不匹配，不是布尔型。" + items[kk] );
                                errCnt++;
                            }
                        }
                    }
                }
            }

            return errCnt ;
        }

        private void AddLog( string str , bool msgBx = true )
        {
            listBox1.Items.Add(str);

            if (msgBx && checkBox1.CheckState == CheckState.Checked )
                MessageBox.Show(str);
        }

        bool IsInt( string str )
        {
            int c = str.Length;
            if( c < 1 )
                return false;

            for( int i= 0 ; i  < c ; i ++ )
            {
                if (  (str[i] < '0' || str[i] > '9') && str[i] != '-' )
                    return false;
            }

            return true;
        }

        bool IsFloat( string str )
        {
            int c = str.Length;
            if (c < 1)
                return false;

            for( int i = 0 ; i< c ; i ++ )
            {
                if ((str[i] < '0' || str[i] > '9') && str[i] != '.' && str[i] != '-' )
                    return false;
            }

            return true;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            //
            if( textBox1.Text.Length > 0 )
            {
                defaultPath = textBox1.Text;
                WriteDefaultPath();
            }

            if (defaultPath == null || defaultPath.Length < 1)
                return;

            int totalErrCnt = 0;
            string[] pths = System.IO.Directory.GetFiles(defaultPath);
            for (int i = 0; i < pths.Length; i++)
            {
                int curErrCnt = CheckFile(pths[i]);

                if( curErrCnt == 0 )
                {
                    int p0 = pths[i].LastIndexOf('/');
                    int p1 = pths[i].LastIndexOf('\\');

                    if (p0 < p1)
                        p0 = p1;

                    string str_ = pths[i];

                    if( p0 < 0 )
                    {
                        // impossible ...
                    }
                    else
                    {
                        str_ = str_.Substring(p0+1);
                    }

                    AddLog(str_ + " 错误为0。值得庆祝.");
                }

                AddLog( "---------------------------------------------------------" );

                totalErrCnt += curErrCnt;
            }

            if( totalErrCnt == 0 )
            {
                MessageBox.Show( "恭喜，这次检查的文件格式全部通过！\n奖励是：\n茶水间冰箱里的盒饭任选一到三份，请自助拿取。\nTHX ！！！" );
            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            //listBox1.Items.Count
            listBox1.Items.Clear();
        }

        string GetTypeStr( string str )
        {
            str = str.ToLower();

            if (str.IndexOf("string") > - 1 )
                return "string";

            if (str.IndexOf("uint") > - 1 )
                return "uint";

            if (str.IndexOf("ubyte") > -1)
                return "ubyte";

            if (str.IndexOf("bool") > -1)
                return "bool";

            if (str.IndexOf("float") > -1)
                return "float";

            if (str.IndexOf("double") > -1)
                return "double";

            if (str.IndexOf("int") > -1)
                return "int";

            if (str.IndexOf("byte") > -1)
                return "byte";

            return "Unknown Type";
        }
    }
}
