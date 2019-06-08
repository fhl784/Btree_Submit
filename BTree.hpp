# include "utility.hpp"
# include <fstream>
# include <functional>
# include <cstddef>
# include "exception.hpp"

namespace sjtu {
	template <class Key, class Value, class Compare = std::less<Key> >
	class BTree {
	public:
		typedef pair <Key, Value> value_type;
		class const_iterator;
		class iterator {
		public:
			size_t offset;
			int weizhi;
			BTree* bpshu;
			iterator() {
				bpshu = NULL;
				weizhi = 0, offset = 0;
			}
			iterator(BTree* shu, size_t preoffset = 0, int preweizhi = 0) {
				bpshu = shu;
				offset = preoffset; weizhi = preweizhi;
			}
			iterator(iterator& other) {
				bpshu = other.bpshu;
				offset = other.offset;
				weizhi = other.weizhi;
			}
			iterator(const_iterator& other) {
				bpshu = other.bpshu;
				offset = other.offset;
				weizhi = other.weizhi;
			}
			Value getValue() {
				yezi p;
				bpshu->du(&p, offset, 1, sizeof(yezi));
				return p.a[weizhi].second;
			}
			OperationResult modify(const Value& value) {
				yezi p;
				bpshu->du(&p, offset, 1, sizeof(yezi));
				p.a[weizhi].second = value;
				bpshu->xie(&p, offset, 1, sizeof(yezi));
				return Success;
			}
			iterator operator++(int) {
				iterator tmp = *this;
				if (*this == bpshu->end())
				{
					bpshu = NULL; weizhi = 0; offset = 0;
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
				else ++weizhi;
				return tmp;
			}
			iterator& operator++() {
				if (*this == bpshu->end()) {
					bpshu = NULL; weizhi = 0; offset = 0;
					return *this;
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
				else ++weizhi;
				return *this;
			}
			iterator operator--(int)
			{
				iterator tmp = *this;
				if (*this == bpshu->begin())
				{
					bpshu = NULL; weizhi = 0; offset = 0;
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
			iterator& operator--()
			{
				if (*this == bpshu->begin())
				{
					bpshu = NULL; weizhi = 0; offset = 0;
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
			};
			class const_iterator
			{
			public:
				size_t offset;
				int weizhi;
				const BTree* bpshu;
			public:
				const_iterator()
				{
					bpshu = NULL;
					weizhi = 0, offset = 0;
				}
				const_iterator(const BTree* shu, size_t preoffset = 0, int preweizhi = 0)
				{
					bpshu = shu;
					offset = preoffset; weizhi = preweizhi;
				}
				const_iterator(const iterator& other)
				{
					bpshu = other.bpshu;
					offset = other.offset;
					weizhi = other.weizhi;
				}
				const_iterator(const const_iterator& other)
				{
					bpshu = other.bpshu;
					offset = other.offset;
					weizhi = other.weizhi;
				}
				Value getValue()
				{
					yezi p;
					bpshu->du(&p, offset, 1, sizeof(yezi));
					return p.a[weizhi].second;
				}
				const_iterator operator++(int)
				{
					const_iterator tmp = *this;
					if (*this == bpshu->cend())
					{
						bpshu = NULL; weizhi = 0; offset = 0;
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
					else ++weizhi;
					return tmp;
				}
				const_iterator& operator++() {
					if (*this == bpshu->cend())
					{
						bpshu = NULL; weizhi = 0; offset = 0;
						return *this;
					}
					yezi p;
					bpshu->du(&p, offset, 1, sizeof(yezi));
					if (weizhi == p.geshu - 1)
					{
						if (p.next == 0) ++weizhi;
						else {
							offset = p.next;
							weizhi = 0;
						}
					}
					else ++weizhi;
					return *this;
				}
				const_iterator operator--(int)
				{
					const_iterator tmp = *this;
					if (*this == bpshu->cbegin())
					{
						bpshu = NULL; weizhi = 0; offset = 0;
						return tmp;
					}
					yezi p, q;
					bpshu->du(&p, offset, 1, sizeof(yezi));
					if (weizhi == 0) {
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
						bpshu = NULL; weizhi = 0; offset = 0;
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
				};
			public:
				static const int M = 2000;
				static const int N = 200;
				static const int Mmin = M / 2;
				static const int Nmin = N / 2;
				static const int position = 0;

				struct name
				{
					char* s;

					name() { s = new char[10]; }

					~name() { if (s != NULL) delete s; }
					void qiming(char* str)
					{
						int j = 0;
						while (str[j] != '\0')
						{
							ch[j] = str[j];
							j++;
						}
						ch[j] = '\0';
					}
				};

				struct xinxi {
					size_t head;
					size_t wei;
					size_t root;
					size_t size;
					size_t wenjianwei;
					xinxi()
					{
						head = 0;
						wei = 0;
						root = 0;
						size = 0;
						wenjianwei = 0;
					}
				};
				struct yezi {
					size_t offset;
					size_t par;
					size_t pre, next;
					int geshu;
					value_type a[N + 1];
					yezi()
					{
						offset = 0, par = 0, pre = 0, next = 0, geshu = 0;
					}
				};
				struct mid {
					size_t offset;
					size_t par;
					size_t s[M + 1];
					Key key[M + 1];
					int geshu;
					bool bz;
					mid()
					{
						offset = 0, par = 0;
						for (int j = 0; j <= M; ++j) s[j] = 0;
						geshu = 0;
						bz = false;
					}
				};


				FILE* filename;
				bool kai;
				name wenjianming;
				xinxi info;
				bool flag;

				inline void dakai() {
					flag = 1;
					if (kai == false) {
						fp = fopen(wenjianming.s, "rb+");
						if (fp == NULL) {
							flag = 0;
							fp = fopen(wenjianming.s, "w");
							fclose(fp);
							fp = fopen(wenjianming.s, "rb+");
						}
						else du(&info, position, 1, sizeof(xinxi));
						kai = true;
					}
				}

				inline void guanbi() {
					if (kai == true) {
						fclose(fp);
						kai = false;
					}
				}

				inline void du(void* weizhi, size_t offset, size_t num, size_t size) const {
					if (fseek(fp, offset, SEEK_SET)) throw "tmd kai bu liao";
					fread(weizhi, size, num, fp);
				}

				inline void xie(void* weizhi, size_t offset, size_t num, size_t size) const {
					if (fseek(fp, offset, SEEK_SET)) throw "tmd kai bu liao";
					fwrite(weizhi, size, num, fp);
				}

				name prename;
				FILE* pret;

				inline void copydufile(void* weizhi, size_t offset, size_t num, size_t size) const {
					if (fseek(pret, offset, SEEK_SET)) throw "tmd kai bu liao";
					size_t tmp = fread(weizhi, num, size, pret);
				}

				size_t yetp;

				inline void copyyezi(size_t offset, size_t prevoffset, size_t parpreoffset) {
					yezi ye, preshu, preyezi;
					copydufile(&preshu, prevoffset, 1, sizeof(yezi));
					ye.offset = offset;
					ye.par = parpreoffset;
					ye.geshu = preshu.geshu; ye.pre = yetp; ye.next = 0;
					if (yetp != 0) {
						du(&preyezi, yetp, 1, sizeof(yezi));
						preyezi.next = offset;
						xie(&preyezi, yetp, 1, sizeof(yezi));
						info.wei = offset;
					}
					else info.head = offset;
					for (int j = 0; j < ye.geshu; ++j) ye.a[j].first = preshu.a[j].first, ye.a[j].second = preshu.a[j].second;
					xie(&ye, offset, 1, sizeof(yezi));
					info.wenjianwei += sizeof(yezi);
					yetp = offset;
				}

				inline void copymid(size_t offset, size_t prevoffset, size_t parpreoffset) {
					mid node, nodeshu;
					copydufile(&nodeshu, prevoffset, 1, sizeof(mid));
					xie(&node, offset, 1, sizeof(mid));
					info.wenjianwei += sizeof(mid);
					node.offset = offset; node.par = parpreoffset;
					node.geshu = nodeshu.geshu; node.bz = nodeshu.bz;
					for (int j = 0; j < node.geshu; ++j) {
						node.key[j] = nodeshu.key[j];
						if (node.bz == true) {
							copyyezi(info.wenjianwei, nodeshu.s[j], offset);
						}
						else {
							copymid(info.wenjianwei, nodeshu.s[j], offset);
						}
					}
					xie(&node, offset, 1, sizeof(mid));
				}

				inline void copyfile(char* to, char* bpshu) {
					prename.qiming(bpshu);
					pret = fopen(prename.s, "rb+");
					if (pret == NULL) throw "no such file";
					xinxi infor;
					copydufile(&infor, position, 1, sizeof(xinxi));
					yetp = 0; info.size = infor.size;
					info.root = info.wenjianwei = sizeof(xinxi);
					xie(&info, position, 1, sizeof(xinxi));
					copymid(info.root, infor.root, 0);
					xie(&info, position, 1, sizeof(xinxi));
					fclose(pret);
				}
				size_t posyezi(const Key & key, size_t offset) {
					mid p;
					du(&p, offset, 1, sizeof(mid));
					if (p.bz == true) {
						int i = 0;
						for (; i < p.geshu; ++i)
							if (key < p.key[i]) break;
						if (i == 0) return 0;
						return p.s[i - 1];
					}
					else {
						int i = 0;
						for (; i < p.geshu; ++i)
							if (key < p.key[i]) break;
						if (i == 0) return 0;
						return posyezi(key, p.s[i - 1]);
					}
				}
				inline void bulidt() {
					info.size = 0;
					info.wenjianwei = sizeof(xinxi);
					mid root;
					yezi ye;
					info.root = root.offset = info.wenjianwei;
					info.wenjianwei += sizeof(mid);
					info.head = info.wei = ye.offset = info.wenjianwei;
					info.wenjianwei += sizeof(yezi);
					root.par = 0;
					root.geshu = 1;
					root.bz = true;
					root.s[0] = ye.offset;
					ye.par = root.offset;
					ye.next = ye.pre = 0;
					ye.geshu = 0;
					xie(&info, position, 1, sizeof(xinxi));
					xie(&root, root.offset, 1, sizeof(mid));
					xie(&ye, ye.offset, 1, sizeof(yezi));
				}
				pair <iterator, OperationResult> charuyezi(yezi & ye, const Key & key, const Value & value) {
					iterator tmp;
					int i = 0;
					for (; i < ye.geshu; ++i) {
						if (key == ye.a[i].first) return pair <iterator, OperationResult>(iterator(NULL), Fail);
						if (key < ye.a[i].first) break;
					}
					for (int j = ye.geshu - 1; j >= i; --j)
						ye.a[j + 1].first = ye.a[j].first, ye.a[j + 1].second = ye.a[j].second;
					ye.a[i].first = key; ye.a[i].second = value;
					++ye.geshu;
					++info.size;
					tmp.bpshu = this; tmp.weizhi = i; tmp.offset = ye.offset;
					xie(&info, position, 1, sizeof(xinxi));
					if (ye.geshu <= N) xie(&ye, ye.offset, 1, sizeof(yezi));
					else fenyezi(ye, tmp, key);
					return pair <iterator, OperationResult>(tmp, Success);
				}
				void insert_node(mid & node, const Key & key, size_t s) {
					int i = 0;
					for (; i < node.geshu; ++i)
						if (key < node.key[i]) break;
					for (int j = node.geshu - 1; j >= i; --j)	node.key[j + 1] = node.key[j];
					for (int j = node.geshu - 1; j >= i; --j) node.s[j + 1] = node.s[j];
					node.key[i] = key;
					node.s[i] = s;
					++node.geshu;
					if (node.geshu <= M) xie(&node, node.offset, 1, sizeof(mid));
					else fenmid(node);
				}

				void fenyezi(yezi & ye, iterator & it, const Key & key) {
					yezi newyezi;
					newyezi.geshu = ye.geshu - (ye.geshu >> 1);
					ye.geshu = ye.geshu >> 1;
					newyezi.offset = info.wenjianwei;
					info.wenjianwei += sizeof(yezi);
					newyezi.par = ye.par;
					for (int j = 0; j < newyezi.geshu; ++j) {
						newyezi.a[j].first = ye.a[j + ye.geshu].first, newyezi.a[j].second = ye.a[j + ye.geshu].second;
						if (newyezi.a[j].first == key) {
							it.offset = newyezi.offset;
							it.weizhi = j;
						}
					}
					newyezi.next = ye.next;
					newyezi.pre = ye.offset;
					ye.next = newyezi.offset;
					yezi nextyezi;
					if (newyezi.next == 0) info.wei = newyezi.offset;
					else {
						du(&nextyezi, newyezi.next, 1, sizeof(yezi));
						nextyezi.pre = newyezi.offset;
						xie(&nextyezi, nextyezi.offset, 1, sizeof(yezi));
					}

					xie(&ye, ye.offset, 1, sizeof(yezi));
					xie(&newyezi, newyezi.offset, 1, sizeof(yezi));
					xie(&info, position, 1, sizeof(xinxi));
					mid par;
					du(&par, ye.par, 1, sizeof(mid));
					insert_node(par, newyezi.a[0].first, newyezi.offset);
				}
				void fenmid(mid & node) {
					mid newnode;
					newnode.geshu = node.geshu - (node.geshu >> 1);
					node.geshu >>= 1;
					newnode.par = node.par;
					newnode.bz = node.bz;
					newnode.offset = info.wenjianwei;
					info.wenjianwei += sizeof(mid);
					for (int j = 0; j < newnode.geshu; ++j)
						newnode.key[j] = node.key[j + node.geshu];
					for (int j = 0; j < newnode.geshu; ++j)
						newnode.s[j] = node.s[j + node.geshu];

					yezi ye;
					mid internal;
					for (int j = 0; j < newnode.geshu; ++j) {
						if (newnode.bz == true) {
							du(&ye, newnode.s[j], 1, sizeof(yezi));
							ye.par = newnode.offset;
							xie(&ye, ye.offset, 1, sizeof(yezi));
						}
						else {
							du(&internal, newnode.s[j], 1, sizeof(mid));
							internal.par = newnode.offset;
							xie(&internal, internal.offset, 1, sizeof(mid));
						}
					}

					if (node.offset == info.root) {
						mid newroot;
						newroot.par = 0;
						newroot.bz = false;
						newroot.offset = info.wenjianwei;
						info.wenjianwei += sizeof(mid);
						newroot.geshu = 2;
						newroot.key[0] = node.key[0];
						newroot.s[0] = node.offset;
						newroot.key[1] = newnode.key[0];
						newroot.s[1] = newnode.offset;
						node.par = newroot.offset;
						newnode.par = newroot.offset;
						info.root = newroot.offset;

						xie(&info, position, 1, sizeof(xinxi));
						xie(&node, node.offset, 1, sizeof(mid));
						xie(&newnode, newnode.offset, 1, sizeof(mid));
						xie(&newroot, newroot.offset, 1, sizeof(mid));
					}
					else {
						xie(&info, position, 1, sizeof(xinxi));
						xie(&node, node.offset, 1, sizeof(mid));
						xie(&newnode, newnode.offset, 1, sizeof(mid));

						mid par;
						du(&par, node.par, 1, sizeof(mid));
						insert_node(par, newnode.key[0], newnode.offset);
					}
				}
				OperationResult qiangyou(yezi ye) {
					if (ye.next == 0) return Fail;
					yezi right;
					du(&right, ye.next, 1, sizeof(yezi));
					if (ye.par != right.par) return Fail;
					if (right.geshu <= Nmin) return Fail;


					Key oldkey, newkey;
					oldkey = right.a[0].first;
					newkey = right.a[1].first;
					ye.a[ye.geshu].first = right.a[0].first;
					ye.a[ye.geshu].second = right.a[0].second;
					ye.geshu++;
					right.geshu--;
					for (int j = 0; j < right.geshu; ++j)
						right.a[j].first = right.a[j + 1].first, right.a[j].second = right.a[j + 1].second;
					mid node;
					du(&node, ye.par, 1, sizeof(mid));
					for (int j = 0; j < node.geshu; ++j) {
						if (node.key[j] == oldkey) {
							node.key[j] = newkey;
							break;
						}
					}

					xie(&node, node.offset, 1, sizeof(mid));
					xie(&ye, ye.offset, 1, sizeof(yezi));
					xie(&right, right.offset, 1, sizeof(yezi));
					return Success;
				}

				OperationResult qiangzuo(yezi ye) {
					if (ye.pre == 0) return Fail;
					yezi left;
					du(&left, ye.pre, 1, sizeof(yezi));
					if (ye.par != left.par) return Fail;
					if (left.geshu <= Nmin) return Fail;


					Key oldkey, newkey;
					oldkey = ye.a[0].first;
					newkey = left.a[left.geshu - 1].first;
					for (int j = ye.geshu - 1; j >= 0; --j)
						ye.a[j + 1].first = ye.a[j].first, ye.a[j + 1].second = ye.a[j].second;
					ye.a[0].first = left.a[left.geshu - 1].first;
					ye.a[0].second = left.a[left.geshu - 1].second;
					++ye.geshu;
					--left.geshu;

					mid node;
					du(&node, ye.par, 1, sizeof(mid));
					for (int j = 0; j < node.geshu; ++j) {
						if (node.key[j] == oldkey) {
							node.key[j] = newkey;
							break;
						}
					}

					xie(&node, node.offset, 1, sizeof(mid));
					xie(&ye, ye.offset, 1, sizeof(yezi));
					xie(&left, left.offset, 1, sizeof(yezi));
					return Success;
				}
				OperationResult hebingyou(yezi ye) {
					if (ye.next == 0) return Fail;
					yezi right;
					du(&right, ye.next, 1, sizeof(yezi));
					if (right.par != ye.par) return Fail;
					for (int j = 0; j < right.geshu; ++j)

					{
						ye.a[ye.geshu].first = right.a[j].first;
						ye.a[ye.geshu].second = right.a[j].second;
						++ye.geshu;
					}
					ye.next = right.next;
					if (right.offset == info.wei) {
						info.wei = ye.offset;
						xie(&info, position, 1, sizeof(xinxi));
					}
					else {
						yezi tmp;
						du(&tmp, ye.next, 1, sizeof(yezi));
						tmp.pre = ye.offset;
						xie(&tmp, tmp.offset, 1, sizeof(yezi));
					}
					mid node;
					du(&node, ye.par, 1, sizeof(mid));
					int i = 0;
					for (; i < node.geshu; ++i)
						if (node.key[i] == right.a[0].first) break;
					for (int j = i; j < node.geshu - 1; ++j)
						node.key[j] = node.key[j + 1], node.s[j] = node.s[j + 1];
					node.geshu--;

					xie(&ye, ye.offset, 1, sizeof(yezi));

					if (jianchamid(node) == Success) xie(&node, node.offset, 1, sizeof(mid));
					else tiaonode(node);
					return Success;
				}
				OperationResult hebingzuo(yezi ye) {
					if (ye.pre == 0) return Fail;
					yezi left;
					du(&left, ye.pre, 1, sizeof(yezi));
					if (left.par != ye.par) return Fail;
					for (int j = 0; j < ye.geshu; ++j) left.a[left.geshu].first = ye.a[j].first, left.a[left.geshu].second = ye.a[j].second, ++left.geshu;
					left.next = ye.next;
					if (info.wei == ye.offset) {
						info.wei = left.offset;
						xie(&info, position, 1, sizeof(xinxi));
					}
					else {
						yezi tmp;
						du(&tmp, left.next, 1, sizeof(yezi));
						tmp.pre = left.offset;
						xie(&tmp, tmp.offset, 1, sizeof(yezi));
					}

					mid node;
					du(&node, left.par, 1, sizeof(mid));
					int i = 0;
					for (; i < node.geshu; ++i)
						if (node.key[i] == ye.a[0].first) break;
					for (int j = i; j < node.geshu - 1; ++j)
						node.key[j] = node.key[j + 1], node.s[j] = node.s[j + 1];
					node.geshu--;

					xie(&left, left.offset, 1, sizeof(yezi));
					if (jianchamid(node) == Success) xie(&node, node.offset, 1, sizeof(mid));
					else tiaonode(node);
					return Success;
				}


				inline OperationResult jianchamid(mid node) {
					if (node.par == 0) return Success;
					if (node.geshu >= Mmin) return Success;
					return Fail;
				}

				void tiaoyezi(yezi ye) {
					if (qiangyou(ye) == Success) return;
					if (qiangzuo(ye) == Success) return;
					if (hebingyou(ye) == Success) return;
					if (hebingzuo(ye) == Success) return;
					xie(&ye, ye.offset, 1, sizeof(yezi));
				}

				void tiaonode(mid node) {
					if (qiangyounode(node) == Success) return;
					if (qiangzuonode(node) == Success) return;
					if (hebingyounode(node) == Success) return;
					if (hebingzuonode(node) == Success) return;
					mid par;
					du(&par, node.par, 1, sizeof(mid));
					if (par.par == 0) {
						info.root = node.offset;
						node.par = 0;
						xie(&info, position, 1, sizeof(xinxi));
						xie(&node, node.offset, 1, sizeof(mid));
					}
					else {
						mid prepar;
						du(&prepar, par.par, 1, sizeof(mid));
						for (int j = 0; j < prepar.geshu; ++j)
							if (prepar.s[j] == par.offset) {
								prepar.s[j] = node.offset;
								break;
							}
						node.par = prepar.offset;
						xie(&prepar, prepar.offset, 1, sizeof(mid));
						xie(&node, node.offset, 1, sizeof(mid));
					}
				}

				OperationResult qiangyounode(mid node) {
					if (node.par == 0) return Fail;
					mid par;
					du(&par, node.par, 1, sizeof(mid));
					int i = 0;
					for (; i < par.geshu; ++i)
						if (par.s[i] == node.offset) break;
					if (i == par.geshu - 1) return Fail;
					mid right;
					du(&right, par.s[i + 1], 1, sizeof(mid));
					if (right.geshu <= Mmin) return Fail;

					node.key[node.geshu] = right.key[0];
					node.s[node.geshu] = right.s[0];
					++node.geshu;
					for (int j = 0; j < right.geshu - 1; ++j) right.key[j] = right.key[j + 1];
					for (int j = 0; j < right.geshu - 1; ++j) right.s[j] = right.s[j + 1];
					--right.geshu;

					par.key[i + 1] = right.key[0];


					if (node.bz == true) {
						yezi child;
						du(&child, node.s[node.geshu - 1], 1, sizeof(yezi));
						child.par = node.offset;
						xie(&child, child.offset, 1, sizeof(yezi));
					}
					else {
						mid child;
						du(&child, node.s[node.geshu - 1], 1, sizeof(mid));
						child.par = node.offset;
						xie(&child, child.offset, 1, sizeof(mid));
					}

					xie(&node, node.offset, 1, sizeof(mid));
					xie(&right, right.offset, 1, sizeof(mid));
					xie(&par, par.offset, 1, sizeof(mid));
					return Success;
				}

				OperationResult qiangzuonode(mid node) {
					if (node.par == 0) return Fail;
					mid par;
					du(&par, node.par, 1, sizeof(mid));
					int i = 0;
					for (; i < par.geshu; ++i)
						if (par.s[i] == node.offset) break;
					if (i == 0) return Fail;
					mid left;
					du(&left, par.s[i - 1], 1, sizeof(mid));
					if (left.geshu <= Mmin) return Fail;

					for (int j = node.geshu - 1; j >= 0; --j) node.key[j + 1] = node.key[j];
					for (int j = node.geshu - 1; j >= 0; --j) node.s[j + 1] = node.s[j];
					node.key[0] = left.key[left.geshu - 1];
					node.s[0] = left.s[left.geshu - 1];
					++node.geshu;
					--left.geshu;

					par.key[i] = node.key[0];

					if (node.bz == true) {
						yezi child;
						du(&child, node.s[0], 1, sizeof(yezi));
						child.par = node.offset;
						xie(&child, child.offset, 1, sizeof(yezi));
					}
					else {
						mid child;
						du(&child, node.s[0], 1, sizeof(mid));
						child.par = node.offset;
						xie(&child, child.offset, 1, sizeof(mid));
					}

					xie(&node, node.offset, 1, sizeof(mid));
					xie(&left, left.offset, 1, sizeof(mid));
					xie(&par, par.offset, 1, sizeof(mid));
					return Success;
				}


				OperationResult hebingyounode(mid node) {
					if (node.par == 0) return Fail;
					mid par;
					du(&par, node.par, 1, sizeof(mid));
					int i = 0;
					for (; i < par.geshu; ++i)
						if (par.s[i] == node.offset) break;
					if (i == par.geshu - 1) return Fail;
					mid right;
					du(&right, par.s[i + 1], 1, sizeof(mid));

					for (int j = 0; j < right.geshu; ++j) {
						node.key[node.geshu] = right.key[j];
						node.s[node.geshu] = right.s[j];
						if (node.bz == true) {
							yezi child;
							du(&child, node.s[node.geshu], 1, sizeof(yezi));
							child.par = node.offset;
							xie(&child, child.offset, 1, sizeof(yezi));
						}
						else {
							mid child;
							du(&child, node.s[node.geshu], 1, sizeof(mid));
							child.par = node.offset;
							xie(&child, child.offset, 1, sizeof(mid));
						}
						++node.geshu;
					}

					for (int j = i + 1; j < par.geshu - 1; ++j)
						par.key[j] = par.key[j + 1], par.s[j] = par.s[j + 1];
					--par.geshu;
					xie(&node, node.offset, 1, sizeof(mid));
					if (jianchamid(par) == Success) xie(&par, par.offset, 1, sizeof(mid));
					else tiaonode(par);
					return Success;
				}
				OperationResult hebingzuonode(mid node) {
					if (node.par == 0) return Fail;
					mid par;
					du(&par, node.par, 1, sizeof(mid));
					int i = 0;
					for (; i < par.geshu; ++i)
						if (par.s[i] == node.offset) break;
					if (i == 0) return Fail;
					mid left;
					du(&left, par.s[i - 1], 1, sizeof(mid));

					for (int j = 0; j < node.geshu; ++j) {
						left.key[left.geshu] = node.key[j];
						left.s[left.geshu] = node.s[j];
						if (left.bz == true) {
							yezi child;
							du(&child, left.s[left.geshu], 1, sizeof(yezi));
							child.par = left.offset;
							xie(&child, child.offset, 1, sizeof(yezi));
						}
						else {
							mid child;
							du(&child, left.s[left.geshu], 1, sizeof(mid));
							child.par = left.offset;
							xie(&child, child.offset, 1, sizeof(mid));
						}
						++left.geshu;
					}

					for (int j = i; j < par.geshu - 1; ++j)
						par.key[j] = par.key[j + 1], par.s[j] = par.s[j + 1];
					--par.geshu;
					xie(&left, left.offset, 1, sizeof(mid));
					if (jianchamid(par) == Success) xie(&par, par.offset, 1, sizeof(mid));
					else tiaonode(par);
					return Success;
				}

				class const_iterator {
					friend class BTree;
				public:
					size_t offset;
					int weizhi;
					const BTree* bpshu;
				public:
					const_iterator() {
						bpshu = NULL;
						weizhi = 0, offset = 0;
					}
					const_iterator(const BTree* shu, size_t preoffset = 0, int preweizhi = 0) {
						bpshu = shu;
						offset = preoffset; weizhi = preweizhi;
					}
					const_iterator(const iterator& other) {
						bpshu = other.bpshu;
						offset = other.offset;
						weizhi = other.weizhi;
					}
					const_iterator(const const_iterator& other) {
						bpshu = other.bpshu;
						offset = other.offset;
						weizhi = other.weizhi;
					}
					Value getValue() {
						yezi p;
						bpshu->du(&p, offset, 1, sizeof(yezi));
						return p.a[weizhi].second;
					}
					const_iterator operator++(int) {
						const_iterator tmp = *this;
						if (*this == bpshu->cend()) {
							bpshu = NULL; weizhi = 0; offset = 0;
							return tmp;
						}
						yezi p;
						bpshu->du(&p, offset, 1, sizeof(yezi));
						if (weizhi == p.geshu - 1) {
							if (p.next == 0) ++weizhi;
							else {
								offset = p.next;
								weizhi = 0;
							}
						}
						else ++weizhi;
						return tmp;
					}
					const_iterator& operator++() {
						if (*this == bpshu->cend()) {
							bpshu = NULL; weizhi = 0; offset = 0;
							return *this;
						}
						yezi p;
						bpshu->du(&p, offset, 1, sizeof(yezi));
						if (weizhi == p.geshu - 1) {
							if (p.next == 0) ++weizhi;
							else {
								offset = p.next;
								weizhi = 0;
							}
						}
						else ++weizhi;
						return *this;
					}
					const_iterator operator--(int) {
						const_iterator tmp = *this;
						if (*this == bpshu->cbegin()) {
							bpshu = NULL; weizhi = 0; offset = 0;
							return tmp;
						}
						yezi p, q;
						bpshu->du(&p, offset, 1, sizeof(yezi));
						if (weizhi == 0) {
							offset = p.pre;
							bpshu->du(&q, p.pre, 1, sizeof(yezi));
							weizhi = q.geshu - 1;
						}
						else --weizhi;
						return tmp;
					}
					const_iterator& operator--() {
						if (*this == bpshu->cbegin()) {
							bpshu = NULL; weizhi = 0; offset = 0;
							return *this;
						}
						yezi p, q;
						bpshu->du(&p, offset, 1, sizeof(yezi));
						if (weizhi == 0) {
							offset = p.pre;
							bpshu->du(&q, p.pre, 1, sizeof(yezi));
							weizhi = q.geshu - 1;
						}
						else --weizhi;
						return *this;
					}
					bool operator==(const iterator& rhs) const {
						return offset == rhs.offset && weizhi == rhs.weizhi && bpshu == rhs.bpshu;
					}
					bool operator==(const const_iterator& rhs) const {
						return offset == rhs.offset && weizhi == rhs.weizhi && bpshu == rhs.bpshu;
					}
					bool operator!=(const iterator& rhs) const {
						return offset != rhs.offset || weizhi != rhs.weizhi || bpshu != rhs.bpshu;
					}
					bool operator!=(const const_iterator& rhs) const {
						return offset != rhs.offset || weizhi != rhs.weizhi || bpshu != rhs.bpshu;
					}
				};

				BTree() {
					fp = NULL;
					dakai();
					if (flag == 0) bulidt();
				}

				BTree(const BTree & other) {
					dakai();
					copyfile(wenjianming.s, other.wenjianming.s);
				}

				BTree& operator=(const BTree & other) {
					dakai();
					copyfile(wenjianming.s, other.wenjianming.s);
				}

				~BTree() {
					guanbi();
				}
				pair <iterator, OperationResult> insert(const Key & key, const Value & value) {
					size_t leafpreoffset = posyezi(key, info.root);
					yezi ye;
					if (info.size == 0 || leafpreoffset == 0) {
						du(&ye, info.head, 1, sizeof(yezi));
						pair <iterator, OperationResult> tmp = charuyezi(ye, key, value);
						if (tmp.second == Fail) return tmp;
						size_t offset = ye.par;
						mid node;
						while (offset != 0) {
							du(&node, offset, 1, sizeof(mid));
							node.key[0] = key;
							xie(&node, offset, 1, sizeof(mid));
							offset = node.par;
						}
						return tmp;
					}
					du(&ye, leafpreoffset, 1, sizeof(yezi));
					pair <iterator, OperationResult> tmp = charuyezi(ye, key, value);
					return tmp;
				}
				OperationResult erase(const Key & key) {
					size_t leafpreoffset = posyezi(key, info.root);
					if (leafpreoffset == 0) return Fail;
					yezi ye;
					du(&ye, leafpreoffset, 1, sizeof(yezi));
					int i = 0;
					for (; i < ye.geshu; ++i)
						if (ye.a[i].first == key) break;
					if (i == ye.geshu) return Fail;
					for (int j = i + 1; j < ye.geshu; ++j)
						ye.a[j - 1].first = ye.a[j].first, ye.a[j - 1].second = ye.a[j].second;
					ye.geshu--;
					size_t internalpreoffset = ye.par;
					mid node;
					while (i == 0) {
						if (internalpreoffset == 0) break;
						du(&node, internalpreoffset, 1, sizeof(mid));
						i = 0;
						for (; i < node.geshu; ++i)
							if (node.key[i] == key) break;
						node.key[i] = ye.a[0].first;
						xie(&node, node.offset, 1, sizeof(mid));
						internalpreoffset = node.par;
					}
					info.size--;
					xie(&info, position, 1, sizeof(xinxi));
					if (ye.geshu < Nmin) {
						tiaoyezi(ye);
						return Success;
					}
					xie(&ye, leafpreoffset, 1, sizeof(yezi));
					return Success;
				}

				iterator begin() {
					return iterator(this, info.head, 0);
				}
				const_iterator cbegin() const {
					return const_iterator(this, info.head, 0);
				}
				iterator end() {
					yezi wei;
					du(&wei, info.wei, 1, sizeof(yezi));
					return iterator(this, info.wei, wei.geshu);
				}
				const_iterator cend() const {
					yezi wei;
					du(&wei, info.wei, 1, sizeof(yezi));
					return const_iterator(this, info.wei, wei.geshu);
				}
				bool empty() const { return info.size == 0; }
				size_t size() const { return info.size; }
				void clear() {
					fp = fopen(wenjianming.s, "w");
					fclose(fp);
					dakai();
					bulidt();
				}
				size_t count(const Key & key) const {
					return static_cast <size_t> (find(key) != iterator(NULL));
				}
				Value at(const Key & key) {
					iterator it = find(key);
					yezi ye;
					if (it == end()) {
						throw "sb";
					}
					du(&ye, it.offset, 1, sizeof(yezi));
					return ye.a[it.weizhi].second;
				}
				iterator find(const Key & key) {
					size_t leafpreoffset = posyezi(key, info.root);
					if (leafpreoffset == 0) return end();
					yezi ye;
					du(&ye, leafpreoffset, 1, sizeof(yezi));
					for (int j = 0; j < ye.geshu; ++j)
						if (ye.a[j].first == key) return iterator(this, leafpreoffset, j);
					return end();
				}
				const_iterator find(const Key & key) const {
					size_t leafpreoffset = posyezi(key);
					if (leafpreoffset == 0) return cend();
					yezi ye;
					du(&ye, leafpreoffset, 1, sizeof(yezi));
					for (int j = 0; j < ye.geshu; ++j)
						if (ye.a[j].first == key) return const_iterator(this, leafpreoffset, j);
					return cend();
				}
			};
		}