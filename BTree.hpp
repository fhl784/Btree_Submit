//
// Created by 郑文鑫 on 2019-03-09.
//

#include "utility.hpp"
#include <functional>
#include <cstddef>
#include "exception.hpp"
#include <map>
namespace sjtu 
{
    template <class Key, class Value, class Compare = std::less<Key> >
	class BTree {
	public:
		typedef pair<const Key, Value> value_type;
		int M = 2000;
		int N = 200;
		int Mmin = 1000;
		int Nmin = 100;
		int pos = 0;
		class yezi
		{
			int offset;
			int par;
			int pre;
			int next;
			int geshu;
			value_type a[N + 1];
			yezi()
			{
				offset = 0;
				fu = 0;
				pre = 0;
				next = 0;
				geshu = 0;
			}
		};
		class mid
		{
			int offset;
			int par;
			int geshu;
			bool bz;
			int erzi[M + 1];
			Key key[M + 1];
			mid()
			{
				offset = 0;
				fu = 0;
				geshu = 0;
				bz = false;
				for (i = 0; i <= M; ++i)
				{
					erzi[i] = 0;
				}
			}
		};
		class xinxi
		{

			int head;
			int wei;
			int size;
			int wenjianwei;
			int root;
			xinxi()
			{
				head = 0;
				wei = 0;
				size = 0;
				wenjianwei = 0;
				root = 0;
			}
		};
		class name
		{
			char* ch;
			name()
			{
				ch = new char[10];
			}
			~name()
			{
				if (ch != NULL) delete ch;
			}
			void qiming(char* str)
			{
				int i = 0;
				while (str[i] != '\0')
				{
					ch[i] = str[i];
					i++;
				}
				ch[i] = '\0';
			}
		};
		FILE* wenjian;
		bool kai;
		name filename;
		xinxi info;
		bool flag;
		name prefilename;
		FILE* prefile;
		int tmp;
		void dakai()
		{
			flag = true;
			if (kai == 0)
			{
				wenjian = fopen(filename.ch, "rb+");
				if (wenjian == NULL)
				{
					flag = false;
					wenjian = fopen(filename.ch, "w");
					fclose(wenjian);
					fp = fopen(filename.ch, "rb+");
				}
				else
				{
					du(&info, pos, 1, sizeof(xinxi));
				}
				kai = true;
			}
		}
		void guanbi()
		{
			if (kai == 1)
			{
				fclose(wenjian);
				kai = false;
			}
		}
		void du(void* dizhi, int offset, int num, int size)
		{
			if (fseek(wenjian, offset, SEEK_SET)) throw "tmd da bu kai";
			fread(dizhi, size, num, wenjian);
		}
		 void xie(void* dizhi, int offset, int num, int size)
		{
			if (fseek(wenjian, offset, SEEK_SET)) throw "tmd da bu kai";
			fwrite(dizhi, size, num, wenjian);
		}
		 void copydufile(void* dizhi, int offset, int num, int size)
		{
			if (fseek(prefile, offset, SEEK_SET)) throw "tmd da bu kai";
			int x = fread(dizhi, num, size, prefile);
		}
		 void copyyezi(int offset, int preoffset, int paroffset)
		{
			yezi ye;
			yezi preye;
			yezi fromye;
			copydufile(&fromye, preoffset, 1, sizeof(yezi));
			ye.offset = offset;
			ye.par = paroffsset;
			ye.geshu = fromye.geshu;
			ye.pre = tmp;
			ye.next = 0;
			if (tmp != 0)
			{
				du(&preye, tmp, 1, sizeof(yezi));
				preye.next = offset;
				xie(&preye, tmp, 1, sizeof(ye));
				info.wei = offset;
			}
			else
			{
				info.head = offset;
			}
			for (int i = 0; i < ye.geshu; ++i)
			{
				ye.a[i].first = fromye.a[i].first;
				ye.a[i].second = fromye.a[i].second;
			}
			xie(&ye, offset, 1, sizeof(yezi));
			info.wenjianwei += sizeof(yezi);
			tmp = offset;
		}
		 void copynode(int offset, int fromoffset, int paroffset)
		{
			mid node, fromnode;
			copydufile(&fromnode, fromoffset, 1, sizeof(mid));
			xie(&node, offset, 1, sizeof(mid));
			info.weijianwei += sizeof(mid);
			node.offset = offset;
			node.par = paroffset;
			node.geshu = fromnode.geshu;
			node.bz = fromnode.bz;
			for (i = 0; i < node.geshu; ++i)
			{
				node.key[i] = fromnode.key[i];
				if (node.bz == true)
				{
					copyleaf(info.wenjianwei, fromnode.erzi[i], offset);
				}
				else
				{
					copynode(info.wenjianwei, fromnode.erzi[i], offset);
				}
			}
			xie(&node, offset, 1, sizeof(mid));
		}
		 void copyfile(char* mo, char* qi)
		{
			xinxi infor;
			prefilename.qiming(qi);
			prefile = fopen(prefilename.ch, "rb+");
			if (prefile == NULL) throw "mei zhe wan yi";
			copydufile(&infor, pos, 1, sizeof(xinxi));
			tmp = 0;
			info.size = infor.size;
			info.wenjianwei = sizeof(xinxi);
			info.root = info.wenjianwei;
			xie(&info, pos, 1, sizeof(xinxi));
			copynode(info.root, infor.foot, 0);
			xie(&info, pos, 1, sizeof(xinxi));
			fclose(prefile);
		}
        class const_iterator;
        class iterator {
		public:
			int offset;
			int weizhi;
			BTree* bpshu;
			OperationResult modify(const Value& value){
				yezi p;
				bpshu->du(&p, offset, 1, sizeof(yezi));
				p.a[weizhi].second = value;
				bpshu->xie(&p, offset, 1, sizeof(yezi));
				return Success;
            }
			iterator() {
				bpshu = NULL;
				weizhi = 0;
				offset = 0;
			}
            iterator(const iterator& other) {
				bpshu = other.bpshu;
				offset = other.offset;
				weizhi = other.weizhi;
            }
			iterator(const_iterator& other)
			{
				bpshu = other.bpshu;
				offset = other.offset;
				weizhi = other.weizhi;
			}
			iterator(BTree zuzong, int zoffset = 0, int zweizhi = 0)
			{
				bpshu = zuzong;
				offset = zoffset;
				weizhi = zweizhi;
			}
            // Return a new iterator which points to the n-next elements
            iterator operator++(int) {
				iterator tmp = *this;
				if (tmp == bpshu->end()) {
					bpshu = NULL;
					weizhi = 0;
					offset = 0;
					return tmp;
				}
				yezi p;
				bpshu->du(&p, offset, 1, sizeof(yezi));
				if (weizhi == p.geshu - 1)
				{
					if (p.next == 0) ++weizhi;
					else
					{
						offset = p.next;
						weizhi = 0;
					}
				}
				else weizhi++;
				return tmp;
            }
            iterator& operator++() {
				if (*this == bpshu->end())
				{
					bpshu = NULL;
					weizhi = 0;
					offset = 0;
					return *this;
				}
				yezi p;
				bpshu->du(&p, offset, 1, sizeof(yezi));
				if (weizhi == p.geshu - 1)
				{
					if (p.next == 0)++weizhi;
					else
					{
						offset = p.next;
						weizhi = 0;
					}
				}
				else weizhi++;
				return *this;
            }
            iterator operator--(int) {
				iterator tmp = *this;
				if (tmp == bpshu->begin())
				{
					bpshu = NULL;
					weizhi = 0;
					offset = 0;
					return tmp;
				}
				yezi p, q;
				bpshu->du(&p, offset, 1, sizeof(yezi));
				if (weizhi == 0)
				{
					offset = p.pre;
					bpshu->du(&q, p.pre, 1, sizeof(yezi));
					weizhi = q.geshu - 1;
				}
				else --weizhi;
				return tmp;
            }
            iterator& operator--() {
				if (*this == bpshu->begin())
				{
					bpshu = NULL;
					weizhi = 0;
					offset = 0;
					return *this;
				}
				yezi p, q;
				bpshu->du(&p, offset, 1, sizeof(yezi));
				if (weizhi == 0)
				{
					offset = p.pre;
					bpshu->du(&q, p.pre, 1, sizeof(yezi));
					weizhi = q.geshu - 1;
				}
				else --weizhi;
				return *this;
            }
            // Overloaded of operator '==' and '!='
            // Check whether the iterators are same
            bool operator==(const iterator& rhs) const {
				if (offset == rhs.offset && weizhi == rhs.weizhi && bpshu == rhs.bpshu) return true;
				else return false;
            }
            bool operator==(const const_iterator& rhs) const {
				if (offset == rhs.offset && weizhi == rhs.weizhi && bpshu == rhs.bpshu) return true;
				else return false;
            }
            bool operator!=(const iterator& rhs) const {
				if (offset != rhs.offset || weizhi != rhs.weizhi || bpshu != rhs.bpshu) return true;
				else return false;
            }
            bool operator!=(const const_iterator& rhs) const {
				if (offset != rhs.offset || weizhi != rhs.weizhi || bpshu != rhs.bpshu) return true;
				else return false;
            }
			Value quchu()
			{
				yezi p;
				bpshu->du(&p, offset, 1, sizeof(yezi));
				return p.a[weizhi].second;
			}
        };
        class const_iterator {
		public:
			int offset;
			int weizhi;
			const BTree* bpshu;
			OperationResult modify(const Value& value) {
				yezi p;
				bpshu->du(&p, offset, 1, sizeof(yezi));
				p.a[weizhi].second = value;
				bpshu->xie(&p, offset, 1, sizeof(yezi));
				return Success;
			}
			const_iterator() {
				bpshu = NULL;
				weizhi = 0;
				offset = 0;
			}
			const_iterator(const iterator& other) {
				bpshu = other.bpshu;
				offset = other.offset;
				weizhi = other.weizhi;
			}
			const_iterator(const iterator& other)
			{
				bpshu = other.bpshu;
				offset = other.offset;
				weizhi = other.weizhi;
			}
			const_iterator(BTree zuzong, int zoffset = 0, int zweizhi = 0)
			{
				bpshu = zuzong;
				offset = zoffset;
				weizhi = zweizhi;
			}
			// Return a new iterator which points to the n-next elements
			const_iterator operator++(int) {
				const_iterator tmp = *this;
				if (tmp == bpshu->cend()) {
					bpshu = NULL;
					weizhi = 0;
					offset = 0;
					return tmp;
				}
				yezi p;
				bpshu->du(&p, offset, 1, sizeof(yezi));
				if (weizhi == p.geshu - 1)
				{
					if (p.next == 0) ++weizhi;
					else
					{
						offset = p.next;
						weizhi = 0;
					}
				}
				else weizhi++;
				return tmp;
			}
			const_iterator& operator++() {
				if (*this == bpshu->cend())
				{
					bpshu = NULL;
					weizhi = 0;
					offset = 0;
					return *this;
				}
				yezi p;
				bpshu->du(&p, offset, 1, sizeof(yezi));
				if (weizhi == p.geshu - 1)
				{
					if (p.next == 0)++weizhi;
					else
					{
						offset = p.next;
						weizhi = 0;
					}
				}
				else weizhi++;
				return *this;
			}
			const_iterator operator--(int) {
				const_iterator tmp = *this;
				if (tmp == bpshu->cbegin())
				{
					bpshu = NULL;
					weizhi = 0;
					offset = 0;
					return tmp;
				}
				yezi p, q;
				bpshu->du(&p, offset, 1, sizeof(yezi));
				if (weizhi == 0)
				{
					offset = p.pre;
					bpshu->du(&q, p.pre, 1, sizeof(yezi));
					weizhi = q.geshu - 1;
				}
				else --weizhi;
				return tmp;
			}
			const_iterator& operator--() {
				if (*this == bpshu->cbegin())
				{
					bpshu = NULL;
					weizhi = 0;
					offset = 0;
					return *this;
				}
				yezi p, q;
				bpshu->du(&p, offset, 1, sizeof(yezi));
				if (weizhi == 0)
				{
					offset = p.pre;
					bpshu->du(&q, p.pre, 1, sizeof(yezi));
					weizhi = q.geshu - 1;
				}
				else --weizhi;
				return *this;
			}
			// Overloaded of operator '==' and '!='
			// Check whether the iterators are same
			bool operator==(const iterator& rhs) const {
				if (offset == rhs.offset && weizhi == rhs.weizhi && bpshu == rhs.bpshu) return true;
				else return false;
			}
			bool operator==(const const_iterator & rhs) const {
				if (offset == rhs.offset && weizhi == rhs.weizhi && bpshu == rhs.bpshu) return true;
				else return false;
			}
			bool operator!=(const iterator & rhs) const {
				if (offset != rhs.offset || weizhi != rhs.weizhi || bpshu != rhs.bpshu) return true;
				else return false;
			}
			bool operator!=(const const_iterator & rhs) const {
				if (offset != rhs.offset || weizhi != rhs.weizhi || bpshu != rhs.bpshu) return true;
				else return false;
			}
			Value quchu()
			{
				yezi p;
				bpshu->du(&p, offset, 1, sizeof(yezi));
				return p.a[weizhi].second;
			}
        };
		int posyezi(Key& key, int offset)
		{
			mid p;
			du(&p, offset, 1, sizeof(mid));
			if (p.bz == true)
			{
				int i = 0;
				while (i < p.geshu)
				{
					if (key < p.key[i]) break;
					++i;
				}
				if (i == 0) return 0;
				else
				{
					return posyezi(key, p.erzi[i - 1]);
				}
			}
		}
		pair<iterator, OperationResult> charuyezi(yezi& ye, Key& key, Value& value)
		{
			iterator fz;
			int i,j;
			for (i = 0; i < ye.geshu; ++i)
			{
				if (key == ye.a[i].first) return pair<iterator, OperationResult>(iterator(NULL), Fail);
				if (key < ye.a[i].first) break;
			}
			for (j = ye.geshu - 1; j >= i; --j)
			{
				ye.a[i + 1].first = ye.a[i].first;
				ye.a[i + 1].second = ye.a[i].second;
			}
			ye.a[i].first = key;
			ye.a[i].second = value;
			++ye.geshu;
			++info.size;
			fz.bpshu = this;
			fz.weizhi = i;
			fz.offset = ye.offset;
			xie(&info, pos, 1, sizeof(yezi));
			if (ye.geshu <= L) xie(&ye, ye.offset, 1, sizeof(yezi));
			else fenyezi(ye, fz, key);
			return pair <iterator, OperationResult>(fz, Success);
		}
		void charunode(mid& node, Key& key, int erzi)
		{
			int i;
			for (i = 0; i < node.geshu; ++i)
			{
				if (key < node.key[i]) break;
			}
			int j;
			for (j = node.geshu - 1; j >= i; --j)
			{
				node.key[i + 1] = node.key[i];
				node.erzi[i + 1] = node.erzi[i];
			}
			node.key[i] = key;
			node.erzi[i] = erzi;
			node.geshu++;
			if (node.geshu <= M) xie(&node, node.offset, 1, sizeof(mid));
			else fenmid(node);
		}
		void fenyezi(yezi& ye, iterator& t, Key& key)
		{
			yezi newye;
			newye.geshu = ye.geshu - ye.geshu / 2;
			ye.geshu /= 2;
			newye.offset = info.wenjianwei;
			info.weijianwei += sizeof(yezi);
			newye.par = ye.par;
			int i;
			for (i = 0; i < newye.geshu; ++i)
			{
				newye.a[i].first = ye.a[i + ye.geshu].first;
				newye.a[i].second = ye.a[i + ye.geshu].second;
				if (newye.a[i].first == key)
				{
					t.offset = newye.offset;
					t.weizhi = i;
				}
			}
			newye.next = ye.next;
			newye.pre = ye.offset;
			ye.next = newye.offset;
			yezi nextye;
			if (newye.next == 0) info.wei = newye.offset;
			else
			{
				du(&nextye, newye.next, 1, sizeof(yezi));
				nextye.pre = newye.offset;
				xie(&nextye, nextye.offset, 1, sizeof(yezi));
			}
			xie(&ye, ye.offset, 1, sizeof(yezi));
			xie(&newye, newye.offset, 1, sizeof(yezi));
			xie(&info, pos, 1, sizeof(xinxi));
			mid par;
			du(&par, ye.par, 1, sizeof(mid));
			charunode(par, newye.a[0].first, newye.offset);
		}
		void fenmid(mid& node)
		{
			mid newnode;
			newnode.geshu = node.geshu - node.geshu / 2;
			node.geshu /= 2;
			newnode.par = node.par;
			newnode.bz = node.bz;
			newnode.offset = info.wenjianwei;
			info.wenjianwei += sizeof(mid);
			int i;
			for (i = 0; i < newnode.geshu; ++i)
			{
				newnode.key[i] = node.key[i + node.geshu];
				newnode.erzi[i] = node.erzi[i + node.geshu];
			}
			yezi ye;
			mid midnode;
			for (i = 0; i < newnode.geshu; ++i)
			{
				if (newnode.bz == true)
				{
					du(&ye, newnode.erzi[i], 1, sizeof(yezi));
					ye.par = newnode.offset;
					xie(&ye, ye.offset, 1, sizeof(yezi));
				}
				else
				{
					du(&midnode, newnode.erzi[i], 1, sizeof(mid));
					midnode.par = newnode.offset;
					xie(&midnode, midnode.offset, 1, sizeof(mid));
				}
			}
			if (node.offset == info.root)
			{
				mid newroot;
				newroot.par = 0;
				newroot.bz = false;
				newroot.offset = info.wenjianwei;
				info.wenjianwei += sizeof(mid);
				newroot.geshu = 2;
				newroot.key[0] = node.key[0];
				newroot.erzi[0] = node.offset;
				newroot.key[1] = newnode.key[0];
				newroot.erzi[1] = newnode.offset;
				node.par = newroot.offset;
				newnode.par = newroot.offset;
				info.root = newroot.offset;
				xie(&info, pos, 1, sizeof(xinxi));
				xie(&node, node, offset, 1, sizeof(mid));
				xie(&newnode, newnode.offset, 1, sizeof(mid));
				xie(&newroot, newroot.offset, 1, sizeof(mid));
			}
			else
			{
				xie(&info, pos, 1, sizeof(xinxi));
				xie(&node, node.offset, 1, sizeof(mid));
				xie(&newnode, newnode.offset, 1, sizeof(mid));
				mid parent;
				du(&parent, node.par, 1, sizeof(mid));
				charunode(parent, newnode.key[0], newnode.offset);
			}
		}
		OperationResult qiangyou(yezi &ye)
		{
			if (ye.next == 0) return Fail;
			yezi right;
			du(&right, ye.next, 1, sizeof(yezi));
			if (ye.par != right.par) return fail;
			if (right.geshu <= Nmin) return fail;
			Key oldkey;
			Key newkey;
			oldkey = right.a[0].first;
			newkey = right.a[1].first;
			ye.a[ye.geshu].first = right.a[0].first;
			ye.a[ye.geshu].second = right.a[0].second;
			ye.geshu++;
			right.geshu--;
			int i;
			for (i = 0; i < right.geshu; ++i)
			{
				right.a[i].first = right.a[i + 1].first;
				right.a[i].second = right.a[i + 1].second;
			}
			mid node;
			du(&node, ye.par, 1, sizeof(mid));
			for (i = 0; i < node.geshu; ++i)
			{
				if (node.key[i] == oldkey)
				{
					node.key[i] = newkey;
					break;
				}
			}
			xie(&node, node.offset, 1, sizeof(mid));
			xie(&ye, ye.offset, 1, sizeof(yezi));
			xie(&right, right.offset, 1, sizeof(yezi));
			return Success;
		}
		OperationResult qiangzuo(yezi& ye)
		{
			if (ye.pre == 0) return Fail;
			yezi left;
			du(&left, ye.pre, 1, sizeof(yezi));
			if (ye.par != left.par) return Fail;
			if (left.geshu <= Nmin) return Fail;
			Key oldkey;
			Key newkey;
			oldkey = ye.a[0].first;
			newkey = left.a[left.geshu - 1].first;
			int i;
			for (i = left.geshu - 1; i >= 0; --i)
			{
				ye.a[i + 1].first = ye.a[i].first;
				ye.a[i + 1].second = ye.a[i].second;
			}
			ye.a[0].first = left.a[left.geshu-1].first;
			ye.a[0].second = left.a[left.geshu-1].second;
			ye.geshu++;
			left.geshu--;
			mid node;
			du(&node, ye.par, 1, sizeof(mid));
			for (i = 0; i < node.geshu; ++i)
			{
				if (node.key[i] == oldkey)
				{
					node.key[i] = newkey;
					break;
				}
			}
			xie(&node, node.offset, 1, sizeof(mid));
			xie(&ye, ye.offset, 1, sizeof(yezi));
			xie(&left, left. offset, 1, sizeof(yezi));
			return Success;
		}
		OperationResult hebingyou(yezi ye)
		{
			if (ye.next == 0) return Fail;
			yezi right;
			du(&right, ye.next, 1, sizeof(yezi));
			if (right.par != ye.par) return Fail;
			int i;
			for (i = 0; i < right.geshu; ++i)
			{
				ye.a[ye.geshu].first = right.a[i].first;
				ye.a[ye.geshu].second = right.a[i].second;
				++ye.geshu;
			}
			ye.next = right.next;
			if (right.offset == info.wei)
			{
				info.wei = ye.offset;
				xie(&info, pos, 1, sizeof(xinxi));
			}
			else
			{
				ye p;
				du(&p, ye.next, 1, sizeof(yezi));
				p.pre = ye.offset;
				xie(&p, p.offset, 1, sizeof(yezi));
			}
			mid node;
			du(&node, ye.par, 1, sizeof(mid));
			int i;
			for (i = 0; i < node.geshu; ++i)
			{
				if (node.key[i] == right.a[0].first) break;
			}
			int j;
			for (j = i; j < node.geshu - 1; ++j)
			{
				node.key[i] = node.key[i + 1];
				node.erzi[i] = node.erzi[i + 1];
			}
			node.geshu--;
			xie(&ye, ye.offset, 1, sizeof(yezi));
			if (midjiancha(node) == Success)
			{
				xie(&node, node.offset, 1, sizeof(mid));
			}
			else
			{
				tiaomid(node);
			}
			return Success;
		}
		OperationResult hebingzuo(yezi ye)
		{
			if (ye.pre == 0) return Fail;
			yezi left;
			du(&left, ye.pre, 1, sizeof(yezi));
			if (left.par != ye.par) return Fail;
			int i;
			for (i = 0; i < ye.geshu; ++i)
			{
				left.a[left.geshu].first = ye.a[i].first;
				left.a[left.geshu].second = ye.a[i].second;
				++left.geshu;
			}
			left.next = ye.next;
			if (ye.offset == info.wei)
			{
				info.wei = left.offset;
				xie(&info, pos, 1, sizeof(xinxi));
			}
			else
			{
				ye p;
				du(&p, left.next, 1, sizeof(yezi));
				p.pre = left.offset;
				xie(&p, p.offset, 1, sizeof(yezi));
			}
			mid node;
			du(&node, left.par, 1, sizeof(mid));
			int i;
			for (i = 0; i < node.geshu; ++i)
			{
				if (node.key[i] == ye.a[0].first) break;
			}
			int j;
			for (j = i; j < node.geshu - 1; ++j)
			{
				node.key[i] = node.key[i + 1];
				node.erzi[i] = node.erzi[i + 1];
			}
			node.geshu--;
			xie(&left, left.offset, 1, sizeof(yezi));
			if (midjiancha(node) == Success)
			{
				xie(&node, node.offset, 1, sizeof(mid));
			}
			else
			{
				tiaomid(node);
			}
			return Success;
		}
		OperationResult midjiancha(mid node)
		{
			if (node.par == 0) return Success;
			if (node.geshu >= Mmin) return Success;
			return Fail;
		}
		void tiaoye(yezi ye)
		{
			if (qiangyou(ye) == Success) return;
			if (qiangzuo(ye) == Success) return;
			if (hebingyou(ye) == Success) return;
			if (hebingzuo(ye) == Success) return;
			xie(&ye, ye.offset, 1, sizeof(yezi));
		}
		void tiaomid(mid node)
		{
			if (qiangyounode(node) == Success) return;
			if (qiangzuonode(node) == Success) return;
			if (hebingyounode(node) == Success) return;
			if (hebingzuonode(node) == Success) return;
			mid parent;
			du(&parent, node.par, 1, sizeof(mid));
			if (parent.par == 0)
			{
				info.root = node.offset;
				node.par = 0;
				xie(&info, pos, 1, sizeof(xinxi));
				xie(&node, node.offset, 1, sizeof(mid));
			}
			else
			{
				mid p;
				du(&p, parent.par, 1, sizeof(mid));
				int i;
				for (i = 0; i < p.geshu; ++i)
				{
					if (p.erzi[i] == parent.offset)
					{
						p.erzi[i] = node.offset;
						break;
					}
				}
				node.par = p.offset;
				xie(&p, p.offset, 1, sizeof(mid));
				xie(&node, node.offset, 1, sizeof(mid));
			}
		}
		OperationResult qiangyounode(mid node)
		{
			if (node.par == 0) return Fail;
			mid parent;
			du(&parent, node.par, 1, sizeof(mid));
			int i;
			for (i = 0; i < parent.geshu; ++i)
			{
				if (parent.erzi[i] == node.offset) break;
			}
			if (i == parent.geshu) throw "bao le";
			if (i == parent.geshu - 1) return Fail;
			mid right;
			du(&right, parent.erzi[i + 1], 1, sizeof(mid));
			if (right.geshu <= Mmin) return Fail;
			node.key[node.geshu] = right.key[0];
			node.erzi[node.geshu] = right.erzi[0];
			++node.geshu;
			int j;
			for (j = 0; j < right.geshu - 1; ++j)
			{
				right.key[j] = right.key[j + 1];
				right.erzi[j] = right.erzi[j + 1];
			}
			--right.geshu;
			parent.key[i + 1] = right.key[0];
			if (node.bz == true)
			{
				yezi child;
				du(&child, node.erzi[node.geshu - 1], 1, sizeof(yezi));
				child.par = node.offset;
				xie(&child, child.offset, 1, sizeof(yezi));
			}
			else
			{
				mid child;
				du(&child, node.erzi[node.geshu - 1], 1, sizeof(mid));
				child.par = node.offset;
				xie(&child, child.offset, 1, sizeof(mid));
			}
			xie(&node, node.offset, 1, sizeof(mid));
			xie(&right, right.offset, 1, sizeof(mid));
			xie(&parent, parent.offset, 1, sizeof(mid));
			return Success;
 		}
		OperationResult qiangzuonode(mid node)
		{
			if (node.par == 0) return Fail;
			mid parent;
			du(&parent, node.par, 1, sizeof(mid));
			int i;
			for (i = 0; i < parent.geshu; ++i)
			{
				if (parent.erzi[i] == node.offset) break;
			}
			if (i ==0) return Fail;
			mid left;
			du(&left, parent.erzi[i-1], 1, sizeof(mid));
			if (left.geshu <= Mmin) return Fail;
			int j;
			for (j = node.geshu-1; j >= 0; --j)
			{
				node.key[j + 1] = node.key[j];
				node.erzi[j + 1] = node.erzi[j];
			}
			node.key[0] = left.key[left.geshu - 1];
			node.erzi[0] = left.erzi[left.geshu - 1];
			++node.geshu;
			--left.geshu;
			parent.key[i] = node.key[0];
			if (node.bz == true)
			{
				yezi child;
				du(&child, node.erzi[0], 1, sizeof(yezi));
				child.par = node.offset;
				xie(&child, child.offset, 1, sizeof(yezi));
			}
			else
			{
				mid child;
				du(&child, node.erzi[0], 1, sizeof(mid));
				child.par = node.offset;
				xie(&child, child.offset, 1, sizeof(mid));
			}
			xie(&node, node.offset, 1, sizeof(mid));
			xie(&left, left.offset, 1, sizeof(mid));
			xie(&parent, parent.offset, 1, sizeof(mid));
			return Success;
		}
		OperationResult hebingyounode(mid node)
		{
			if (node.par == 0) return Fail;
			mid par;
			du(&par, node.par, 1, sizeof(mid));
			int i;
			for (i = 0; i < par.geshu; ++i)
			{
				if (par.erzi[i] == node.offset) break;
			}
			if (i == par.geshu) throw "bao le";
			if (i == par.geshu - 1) return Fail;
			mid right;
			du(&right, par.erzi[i + 1], 1, sizeof(mid));
			int j;
			for (j = 0; j < right.geshu; ++j)
			{
				node.key[node.geshu] = right.key[j];
				node.erzi[node.geshu] = right.erzi[j];
				if (node.bz == true)
				{
					yezi child;
					du(&child, node.erzi[node.geshu], 1, sizeof(yezi));
					child.par = node.offset;
					xie(&child, child.offset, 1, sizeof(yezi));
				}
				else
				{
					mid child;
					du(&child, node.erzi[node.geshu], 1, sizeof(mid));
					child.par = node.offset;
					xie(&child, child.offset, 1, sizeof(mid));
				}
				++node.geshu;
			}
			for (j = i + 1; j < par.geshu - 1; ++j)
			{
				par.key[j] = par.key[j + 1];
				par.erzi[j] = par.erzi[j + 1];
			}
			--par.geshu;
			xie(&node, node.offset, 1, sizeof(mid));
			if (midjiancha(par) == Success) xie(&par, par.offset, 1, sizeof(mid));
			else tiaomid(par);
			return Success;
		}
		OperationResult hebingzuonode(mid node)
		{
			if (node.par == 0) return Fail;
			mid par;
			du(&par, node.par, 1, sizeof(mid));
			int i;
			for (i = 0; i < par.geshu; ++i)
			{
				if (par.erzi[i] == node.offset) break;
			}
			if (i == par.geshu) throw "bao le";
			if (i == 0) return Fail;
			mid left;
			du(&left, par.erzi[i-1], 1, sizeof(mid));
			int j;
			for (j = 0; j < right.geshu; ++j)
			{
				left.key[left.geshu] = node.key[j];
				left.erzi[left.geshu] = node.erzi[j];
				if (left.bz == true)
				{
					yezi child;
					du(&child, left.erzi[left.geshu], 1, sizeof(yezi));
					child.par = left.offset;
					xie(&child, child.offset, 1, sizeof(yezi));
				}
				else
				{
					mid child;
					du(&child, left.erzi[left.geshu], 1, sizeof(mid));
					child.par = left.offset;
					xie(&child, child.offset, 1, sizeof(mid));
				}
				++left.geshu;
			}
			for (j = i; j < par.geshu - 1; ++j)
			{
				par.key[j] = par.key[j + 1];
				par.erzi[j] = par.erzi[j + 1];
			}
			--par.geshu;
			xie(&left, left.offset, 1, sizeof(mid));
			if (midjiancha(par) == Success) xie(&par, par.offset, 1, sizeof(mid));
			else tiaomid(par);
			return Success;
		}
		 void buildtree()
		{
			info.size = 0;
			info.wenjianwei = sizeof(xinxi);
			mid root;
			yezi ye;
			root.offset = info.wenjianwei;
			info.root = root.offset;
			info.wenjianwei += sizeof(mid);
			info.head = info.wei = ye.offset = info.eof;
			info.wenjianwei += sizeof(yezi);
			root.par = 0;
			root.geshu = 1;
			root.bz = 1;
			root.erzi[0] = ye.offset;
			ye.par = root.offset;
			ye.next = ye.pre = 0;
			ye.geshu = 0;
			xie(&info, pos, 1, sizeof(xinxi));
			xie(&root, root.offset, 1, sizeof(mid));
			xie(&ye, ye.offset, 1, sizeof(ye));
		}
        // Default Constructor and Copy Constructor
        BTree() {
			filename.ch = "bpt";
			wenjian = NULL;
			dakai();
			if (flag == false) buildtree();
        }
        BTree(const BTree& other) {
			filename.ch = "bpt";
			dakai();
			copyfile(filename.ch, other.filename.ch);
        }
        BTree& operator=(const BTree& other) {
			filename.ch = "bpt";
			dakai();
			copyfile(filename.ch, other.filename.ch);
        }
        ~BTree() {
			guanbi();
        }
        // Insert: Insert certain Key-Value into the database
        // Return a pair, the first of the pair is the iterator point to the new
        // element, the second of the pair is Success if it is successfully inserted
        pair<iterator, OperationResult> insert(const Key& key, const Value& value) {
			int yeoffset = posyezi(key, info.root);
			yezi ye;
			if (info.size == 0 || yeoffset == 0)
			{
				du(&ye, info.head, 1, sizeof(yezi));
				pair <iterator, OperationResult> p = charuyezi(ye, key, value);
				if (p.second == Fail) return p;
				int offset = ye.par;
				mid node;
				while (offset != 0)
				{
					du(&node, offset, 1, sizeof(mid));
					node.key[0] = key;
					xie(&node, offset, 1, sizeof(mid));
					offset = node.par;
				}
				return p;
			}
			du(&ye, yeoffset, 1, sizeof(mid));
			pair <iterator, OperationResult> q = charuyezi(ye, key, value);
			return q;
        }
        // Erase: Erase the Key-Value
        // Return Success if it is successfully erased
        // Return Fail if the key doesn't exist in the database
        OperationResult erase(const Key& key) {
			int yeoffset = posyezi(key, info.root);
			if (yeoffset == 0) return Fail;
			yezi ye;
			du(&ye, yeoffset, 1, sizeof(yezi));
			int i;
			for (i = 0; i < ye.geshu; ++i)
			{
				if (ye.a[i].first == key) break;
			}
			if (i == ye.geshu) return Fail;
			int j;
			for (j = pos + 1; j < ye.geshu; ++j)
			{
				ye.a[j - 1].first = ye.a[j].first;
				ye.a[j - 1].second = ye.a[j - 1].second;
			}
			ye.geshu--;
			int midoffset = ye.par;
			mid node;
			while (i == 0)
			{
				if (midoffset == 0) break;
				du(&node, midoffset, 1, sizeof(mid));
				i = 0;
				for (; i < node.geshu; ++i)
				{
					if (node.key[i] == key) break;
				}
				node.key[i] = ye.a[0].first;
				xie(&node, node.offset, 1, sizeof(mid));
				midoffset = node.par;
			}
			info.size--;
			xie(&info, pos, 1, sizeof(xinxi));
			if (ye.geshu < Nmin)
			{
				tiaoye(ye);
				return Success;
			}
			xie(&ye, yeoffset, 1, sizeof(yezi));
			return Success;
        }
        // Return a iterator to the beginning
        iterator begin() {
			return iterator(this, info.head, 0);
		}
        const_iterator cbegin() const {
			return const_iterator(this, info.head, 0);
		}
        // Return a iterator to the end(the next element after the last)
        iterator end() {
			yezi wei;
			du(&wei, info.wei, 1, sizeof(yezi));
			return iterator(this, info.tail, tail.geshu);
		}
        const_iterator cend() const {
			yezi wei;
			du(&wei, info.wei, 1, sizeof(yezi));
			return const_iterator(this, info.tail, tail.geshu);
		}
        // Check whether this BTree is empty
        bool empty() const {
			if (info.size == 0)
				return true;
			else return false;
		}
        // Return the number of <K,V> pairs
        size_t size() const {
			return info.size;
		}
        // Clear the BTree
        void clear() {
			wenjian = fopen(filename.ch, "w");
			fclose(wenjian);
			dakai();
			buildtree();
		}
        // Return the value refer to the Key(key)
        Value at(const Key& key){
			iterator t= find(key);
			yezi ye;
			if (t == end())
			{
				throw "zha le";
			}
			du(&ye, t.offset, 1, sizeof(yezi));
			return ye.a[t.weizhi].second;
        }
        /**
         * Returns the number of elements with key
         *   that compares equivalent to the specified argument,
         * The default method of check the equivalence is !(a < b || b > a)
         */
        size_t count(const Key& key) const {
			return static_cast<size_t> {find(key) != iterator(NULL); }
		}
        /**
         * Finds an element with key equivalent to key.
         * key value of the element to search for.
         * Iterator to an element with key equivalent to key.
         *   If no such element is found, past-the-end (see end()) iterator is
         * returned.
         */
        iterator find(const Key& key) {
			int yeoffset = posyezi(key, info.root);
			if (yeoffset == 0) return end();
			yezi ye;
			du(&ye, yeoffset, 1, sizeof(yezi));
			int i;
			for (i = 0; i < ye.geshu; ++i)
			{
				if (ye.a[i].first == key) return iterator(this, yeoffset, i);
			}
			return end();
		}
        const_iterator find(const Key& key) const {
			int yeoffset = posyezi(key, info.root);
			if (yeoffset == 0) return cend();
			yezi ye;
			du(&ye, yeoffset, 1, sizeof(yezi));
			int i;
			for (i = 0; i < ye.geshu; ++i)
			{
				if (ye.a[i].first == key) return const_iterator(this, yeoffset, i);
			}
			return cend();
		}
    };
}  // namespace sjtu

