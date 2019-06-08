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
	public:
		class iterator {
		public:
			size_t offset;        
			int weizhi;							
			BTree* bptree;
		public:
			iterator() {
				bptree = NULL;
				weizhi = 0, offset = 0;
			}
			iterator(BTree* zuzong, size_t zuzongoffset = 0, int zuzongweizhi = 0) {
				bptree = zuzong;
				offset = zuzongoffset; weizhi = zuzongweizhi;
			}
			iterator(const iterator& other) {
				bptree = other.bptree;
				offset = other.offset;
				weizhi = other.weizhi;
			}
			iterator(const const_iterator& other) {
				bptree = other.bptree;
				offset = other.offset;
				weizhi = other.weizhi;
			}

			
			Value getValue() {
				yezi p;
				bptree->du(&p, offset, 1, sizeof(yezi));
				return p.a[weizhi].second;
			}

			OperationResult modify(const Value& value) {
				yezi p;
				bptree->du(&p, offset, 1, sizeof(yezi));
				p.a[weizhi].second = value;
				bptree->xie(&p, offset, 1, sizeof(yezi));
				return Success;
			}

		
			iterator operator++(int) {
				iterator tmp = *this;
				if (*this == bptree->end()) 
				{
					bptree = NULL; 
					weizhi = 0; 
					offset = 0;
					return tmp;
				}
				yezi p;
				bptree->du(&p, offset, 1, sizeof(yezi));
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
				if (*this == bptree->end()) 
				{
					bptree = NULL; weizhi = 0; offset = 0;
					return *this;
				}
				yezi p;
				bptree->du(&p, offset, 1, sizeof(yezi));
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
			iterator operator--(int) {
				iterator tmp = *this;
				if (*this == bptree->begin()) {
					bptree = NULL; weizhi = 0; offset = 0;
					return tmp;
				}
				yezi p, q;
				bptree->du(&p, offset, 1, sizeof(yezi));
				if (weizhi == 0) 
				{
					offset = p.pre;
					bptree->du(&q, p.pre, 1, sizeof(yezi));
					weizhi = q.geshu - 1;
				}
				else --weizhi;
				return tmp;
			}
			iterator& operator--() {
				if (*this == bptree->begin()) 
				{
					bptree = NULL;
					weizhi = 0;
					offset = 0;
					return *this;
				}
				yezi p, q;
				bptree->du(&p, offset, 1, sizeof(yezi));
				if (weizhi == 0) {
					offset = p.pre;
					bptree->du(&q, p.pre, 1, sizeof(yezi));
					weizhi = q.geshu - 1;
				}
				else --weizhi;
				return *this;
			}
			bool operator==(const iterator& rhs) const {
				return offset == rhs.offset && weizhi == rhs.weizhi && bptree == rhs.bptree;
			}
			bool operator==(const const_iterator& rhs) const {
				return offset == rhs.offset && weizhi == rhs.weizhi && bptree == rhs.bptree;
			}
			bool operator!=(const iterator& rhs) const {
				return offset != rhs.offset || weizhi != rhs.weizhi || bptree != rhs.bptree;
			}
			bool operator!=(const const_iterator& rhs) const {
				return offset != rhs.offset || weizhi != rhs.weizhi || bptree != rhs.bptree;
			}
		};
		class const_iterator {
			friend class BTree;
		public:
			size_t offset;
			int weizhi;
			const BTree* bptree;
		public:
			const_iterator() {
				bptree = NULL;
				weizhi = 0, offset = 0;
			}
			const_iterator(const BTree* zuzong, size_t zuzongoffset = 0, int zuzongweizhi = 0) {
				bptree = zuzong;
				offset = zuzongoffset; weizhi = zuzongweizhi;
			}
			const_iterator(const iterator& other) {
				bptree = other.bptree;
				offset = other.offset;
				weizhi = other.weizhi;
			}
			const_iterator(const const_iterator& other) {
				bptree = other.bptree;
				offset = other.offset;
				weizhi = other.weizhi;
			}

			Value getValue() {
				yezi p;
				bptree->du(&p, offset, 1, sizeof(yezi));
				return p.a[weizhi].second;
			}

			const_iterator operator++(int) {
				const_iterator tmp = *this;

				if (*this == bptree->cend()) {
					bptree = NULL; weizhi = 0; offset = 0;
					return tmp;
				}
				yezi p;
				bptree->du(&p, offset, 1, sizeof(yezi));
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
				if (*this == bptree->cend()) {
					bptree = NULL; weizhi = 0; offset = 0;
					return *this;
				}
				yezi p;
				bptree->du(&p, offset, 1, sizeof(yezi));
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
				if (*this == bptree->cbegin()) {
					bptree = NULL; weizhi = 0; offset = 0;
					return tmp;
				}
				yezi p, q;
				bptree->du(&p, offset, 1, sizeof(yezi));
				if (weizhi == 0) {
					offset = p.pre;
					bptree->du(&q, p.pre, 1, sizeof(yezi));
					weizhi = q.geshu - 1;
				}
				else --weizhi;
				return tmp;
			}
			const_iterator& operator--() {
				if (*this == bptree->cbegin()) {
					bptree = NULL; weizhi = 0; offset = 0;
					return *this;
				}
				yezi p, q;
				bptree->du(&p, offset, 1, sizeof(yezi));
				if (weizhi == 0) {
					offset = p.pre;
					bptree->du(&q, p.pre, 1, sizeof(yezi));
					weizhi = q.geshu - 1;
				}
				else --weizhi;
				return *this;
			}
			bool operator==(const iterator& rhs) const {
				return offset == rhs.offset && weizhi == rhs.weizhi && bptree == rhs.bptree;
			}
			bool operator==(const const_iterator& rhs) const {
				return offset == rhs.offset && weizhi == rhs.weizhi && bptree == rhs.bptree;
			}
			bool operator!=(const iterator& rhs) const {
				return offset != rhs.offset || weizhi != rhs.weizhi || bptree != rhs.bptree;
			}
			bool operator!=(const const_iterator& rhs) const {
				return offset != rhs.offset || weizhi != rhs.weizhi || bptree != rhs.bptree;
			}
		};

	public:
		static const int M = 1000;               
		static const int N = 200;                 
		static const int Mmin = M / 2;        
		static const int Nmin = N / 2;           
		static const int position = 0;

		struct nameString {
			char* mz;

			nameString() { mz = new char[10]; }

			~nameString() { if (mz != NULL) delete mz; }

			void qiming(int x) {
				mz[0] = 'f';
			}
			void qiming(char* str) {
				int j = 0;
				for (; str[j]; ++j) mz[j] = str[j];
				mz[j] = 0;
			}
		};

		struct xinxi {
			size_t head;        
			size_t wei;         
			size_t root;         
			size_t size;         
			size_t wenjianwei;     
			xinxi() {
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
			yezi() {
				offset = 0;
				par = 0;
				pre = 0;
				next = 0;
				geshu = 0;
			}
		};

		struct mid {
			size_t offset;      	
			size_t par;           
			size_t erzi[M + 1];     	
			Key key[M + 1];  
			int geshu;              
			bool bz;            
			mid() 
			{
				offset = 0, par = 0;
				for (int j = 0; j <= M; ++j) erzi[j] = 0;
				geshu = 0;
				bz = 0;
			}
		};


		FILE* wenjian;
		bool kai;
		nameString filename;
		xinxi info;
		bool flag;

		inline void dakai() {
			flag = 1;
			if (kai == 0) 
			{
				wenjian = fopen(filename.mz, "rb+");
				if (wenjian == NULL) 
				{
					flag = 0;
					wenjian = fopen(filename.mz, "w");
					fclose(wenjian);
					wenjian = fopen(filename.mz, "rb+");
				}
				else du(&info, position, 1, sizeof(xinxi));
				kai = 1;
			}
		}

		inline void closeFile() {
			if (kai == 1) {
				fclose(wenjian);
				kai = 0;
			}
		}

		inline void du(void* weizhi, size_t offset, size_t num, size_t size) const {
			if (fseek(wenjian, offset, SEEK_SET)) throw "nmsl!";
			fread(weizhi, size, num, wenjian);
		}

		inline void xie(void* weizhi, size_t offset, size_t num, size_t size) const {
			if (fseek(wenjian, offset, SEEK_SET)) throw "nmsl!";
			fwrite(weizhi, size, num, wenjian);
		}

		nameString prefilename;
		FILE* prefile;

		inline void copy_readFile(void* weizhi, size_t offset, size_t num, size_t size) const {
			if (fseek(prefile, offset, SEEK_SET)) throw "nmsl";
			size_t tmp = fread(weizhi, num, size, prefile);
		}

		size_t yetp;
		inline void buildt() {
			info.size = 0;
			info.wenjianwei = sizeof(xinxi);
			mid root;
			yezi ye;
			info.root = root.offset = info.wenjianwei;
			info.wenjianwei += sizeof(mid);
			info.head = info.wei = ye.offset = info.wenjianwei;
			info.wenjianwei += sizeof(yezi);
			root.par = 0; root.geshu = 1; root.bz = 1;
			root.erzi[0] = ye.offset;
			ye.par = root.offset;
			ye.next = ye.pre = 0;
			ye.geshu = 0;
			xie(&info, position, 1, sizeof(xinxi));
			xie(&root, root.offset, 1, sizeof(mid));
			xie(&ye, ye.offset, 1, sizeof(yezi));
		}
		inline void copyyezi(size_t offset, size_t preoffset, size_t paroffset) {
			yezi ye, parye, parye;
			copy_readFile(&parye, preoffset, 1, sizeof(yezi));
			ye.offset = offset, ye.par = paroffset;
			ye.geshu = parye.geshu; ye.pre = yetp; ye.next = 0;
			if (yetp != 0) 
			{
				du(&parye, yetp, 1, sizeof(yezi));
				parye.next = offset;
				xie(&parye, yetp, 1, sizeof(yezi));
				info.wei = offset;
			}
			else info.head = offset;
			for (int j = 0; j < ye.geshu; ++j)
			{
				ye.a[j].first = parye.a[j].first;
				ye.a[j].second = parye.a[j].second;
			}
			xie(&ye, offset, 1, sizeof(yezi));
			info.wenjianwei += sizeof(yezi);
			yetp = offset;
		}

		inline void copymid(size_t offset, size_t preoffset, size_t paroffset) {
			mid node, prenode;
			copy_readFile(&prenode, preoffset, 1, sizeof(mid));
			xie(&node, offset, 1, sizeof(mid));
			info.wenjianwei += sizeof(mid);
			node.offset = offset; node.par = paroffset;
			node.geshu = prenode.geshu; node.bz = prenode.bz;
			for (int j = 0; j < node.geshu; ++j) 
			{
				node.key[j] = prenode.key[j];
				if (node.bz == 1) 
				{  				
					copyyezi(info.wenjianwei, prenode.erzi[j], offset);
				}
				else 
				{              
					copymid(info.wenjianwei, prenode.erzi[j], offset);
				}
			}
			xie(&node, offset, 1, sizeof(mid));
		}

		inline void copyfile(char* to, char* bptree) {
			prefilename.qiming(bptree);
			prefile = fopen(prefilename.mz, "rb+");
			if (prefile == NULL) throw "no such file";
			xinxi infoo;
			copy_readFile(&infoo, position, 1, sizeof(xinxi));
			yetp = 0; info.size = infoo.size;
			info.root = info.wenjianwei = sizeof(xinxi);
			xie(&info, position, 1, sizeof(xinxi));
			copymid(info.root, infoo.root, 0);
			xie(&info, position, 1, sizeof(xinxi));
			fclose(prefile);
		}

		size_t posyezi(const Key & key, size_t offset) {
			mid p;
			du(&p, offset, 1, sizeof(mid));
			if (p.bz == 1) {

				int i = 0;
				for (; i < p.geshu; ++i)
					if (key < p.key[i]) break;
				if (i == 0) return 0;
				return p.erzi[i - 1];
			}
			else {
				int i = 0;
				for (; i < p.geshu; ++i)
					if (key < p.key[i]) break;
				if (i == 0) return 0;
				return posyezi(key, p.erzi[i - 1]);
			}
		}

		BTree() {
			filename.qiming(0);
			wenjian = NULL;
			dakai();
			if (flag == 0) buildt();
		}

		BTree(const BTree & other) {
			filename.qiming(0);
			dakai();
			copyfile(filename.mz, other.filename.mz);
		}

		BTree& operator=(const BTree & other) {
			filename.qiming(0);
			dakai();
			copyfile(filename.mz, other.filename.mz);
		}

		~BTree() {
			closeFile();
		}

		pair <iterator, OperationResult> insert(const Key & key, const Value & value) {
			size_t leaf_offset = posyezi(key, info.root);
			yezi ye;
			if (info.size == 0 || leaf_offset == 0) {				
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
			du(&ye, leaf_offset, 1, sizeof(yezi));
			pair <iterator, OperationResult> tmp = charuyezi(ye, key, value);
			return tmp;
		}
		OperationResult erase(const Key & key) {
			size_t leaf_offset = posyezi(key, info.root);
			if (leaf_offset == 0) return Fail;
			yezi ye;
			du(&ye, leaf_offset, 1, sizeof(yezi));
			int i = 0;
			for (; i < ye.geshu; ++i)
				if (ye.a[i].first == key) break;
			if (i == ye.geshu) return Fail;        
		
			for (int j = i + 1; j < ye.geshu; ++j)
				ye.a[j - 1].first = ye.a[j].first, ye.a[j - 1].second = ye.a[j].second;
			ye.geshu--;
		
			size_t internal_offset = ye.par;
			mid node;
			while (i == 0) {
				if (internal_offset == 0) break;
				du(&node, internal_offset, 1, sizeof(mid));
				i = 0;
				for (; i < node.geshu; ++i)
					if (node.key[i] == key) break;
				node.key[i] = ye.a[0].first;
				xie(&node, node.offset, 1, sizeof(mid));
				internal_offset = node.par;
			}
			info.size--;
			xie(&info, position, 1, sizeof(xinxi));
			if (ye.geshu < Nmin) {
				tiaoye(ye);
				return Success;
			}
			xie(&ye, leaf_offset, 1, sizeof(yezi));
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
			wenjian = fopen(filename.mz, "w");
			fclose(wenjian);
			dakai();
			buildt();
		}
		size_t count(const Key & key) const {
			return static_cast <size_t> (find(key) != iterator(NULL));
		}
		Value at(const Key & key) {
			iterator it = find(key);
			yezi ye;
			if (it == end()) {
				throw "not found";
			}
			du(&ye, it.offset, 1, sizeof(yezi));
			return ye.a[it.weizhi].second;
		}
		iterator find(const Key & key) {
			size_t leaf_offset = posyezi(key, info.root);
			if (leaf_offset == 0) return end();
			yezi ye;
			du(&ye, leaf_offset, 1, sizeof(yezi));
			for (int j = 0; j < ye.geshu; ++j)
				if (ye.a[j].first == key) return iterator(this, leaf_offset, j);
			return end();
		}
		const_iterator find(const Key & key) const {
			size_t leaf_offset = posyezi(key);
			if (leaf_offset == 0) return cend();
			yezi ye;
			du(&ye, leaf_offset, 1, sizeof(yezi));
			for (int j = 0; j < ye.geshu; ++j)
				if (ye.a[j].first == key) return const_iterator(this, leaf_offset, j);
			return cend();
		}
		pair <iterator, OperationResult> charuyezi(yezi& ye, const Key& key, const Value& value) {
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
			tmp.bptree = this; tmp.weizhi = i; tmp.offset = ye.offset;
			xie(&info, position, 1, sizeof(xinxi));
			if (ye.geshu <= N) xie(&ye, ye.offset, 1, sizeof(yezi));
			else fenlieyezi(ye, tmp, key);
			return pair <iterator, OperationResult>(tmp, Success);
		}


		void charunode(mid & node, const Key & key, size_t erzi) {
			int i = 0;
			for (; i < node.geshu; ++i)
				if (key < node.key[i]) break;
			for (int j = node.geshu - 1; j >= i; --j)
				node.key[j + 1] = node.key[j];
			for (int j = node.geshu - 1; j >= i; --j)
				node.erzi[j + 1] = node.erzi[j];
			node.key[i] = key;
			node.erzi[i] = erzi;
			++node.geshu;
			if (node.geshu <= M) xie(&node, node.offset, 1, sizeof(mid));
			else fenliemid(node);
		}


		void fenlieyezi(yezi & ye, iterator & it, const Key & key) {
			yezi newye;
			newye.geshu = ye.geshu - (ye.geshu / 2);
			ye.geshu = ye.geshu / 2;
			newye.offset = info.wenjianwei;
			info.wenjianwei += sizeof(yezi);
			newye.par = ye.par;
			for (int j = 0; j < newye.geshu; ++j) {
				newye.a[j].first = ye.a[j + ye.geshu].first, newye.a[j].second = ye.a[j + ye.geshu].second;
				if (newye.a[j].first == key) {
					it.offset = newye.offset;
					it.weizhi = j;
				}
			}

			newye.next = ye.next;
			newye.pre = ye.offset;
			ye.next = newye.offset;
			yezi nextye;
			if (newye.next == 0) info.wei = newye.offset;
			else {
				du(&nextye, newye.next, 1, sizeof(yezi));
				nextye.pre = newye.offset;
				xie(&nextye, nextye.offset, 1, sizeof(yezi));
			}


			xie(&ye, ye.offset, 1, sizeof(yezi));
			xie(&newye, newye.offset, 1, sizeof(yezi));
			xie(&info, position, 1, sizeof(xinxi));


			mid par;
			du(&par, ye.par, 1, sizeof(mid));
			charunode(par, newye.a[0].first, newye.offset);
		}


		void fenliemid(mid & node) {
			mid newnode;
			newnode.geshu = node.geshu - (node.geshu / 2);
			node.geshu /= 2;
			newnode.par = node.par;
			newnode.bz = node.bz;
			newnode.offset = info.wenjianwei;
			info.wenjianwei += sizeof(mid);
			for (int j = 0; j < newnode.geshu; ++j)
				newnode.key[j] = node.key[j + node.geshu];
			for (int j = 0; j < newnode.geshu; ++j)
				newnode.erzi[j] = node.erzi[j + node.geshu];

			yezi ye;
			mid midtp;
			for (int j = 0; j < newnode.geshu; ++j) {
				if (newnode.bz == 1) {
					du(&ye, newnode.erzi[j], 1, sizeof(yezi));
					ye.par = newnode.offset;
					xie(&ye, ye.offset, 1, sizeof(yezi));
				}
				else {
					du(&midtp, newnode.erzi[j], 1, sizeof(mid));
					midtp.par = newnode.offset;
					xie(&midtp, midtp.offset, 1, sizeof(mid));
				}
			}

			if (node.offset == info.root) {

				mid newroot;
				newroot.par = 0;
				newroot.bz = 0;
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
				charunode(par, newnode.key[0], newnode.offset);
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
			for (int j = 0; j < right.geshu; ++j) right.a[j].first = right.a[j + 1].first, right.a[j].second = right.a[j + 1].second;

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
				yezi temp;
				du(&temp, ye.next, 1, sizeof(yezi));
				temp.pre = ye.offset;
				xie(&temp, temp.offset, 1, sizeof(yezi));
			}
			mid node;
			du(&node, ye.par, 1, sizeof(mid));
			int i = 0;
			for (; i < node.geshu; ++i)
				if (node.key[i] == right.a[0].first) break;
			for (int j = i; j < node.geshu - 1; ++j)
			{
				node.key[j] = node.key[j + 1];
				node.erzi[j] = node.erzi[j + 1];
			}
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
				yezi temp;
				du(&temp, left.next, 1, sizeof(yezi));
				temp.pre = left.offset;
				xie(&temp, temp.offset, 1, sizeof(yezi));
			}

			mid node;
			du(&node, left.par, 1, sizeof(mid));
			int i = 0;
			for (; i < node.geshu; ++i)
				if (node.key[i] == ye.a[0].first) break;
			for (int j = i; j < node.geshu - 1; ++j)
				node.key[j] = node.key[j + 1], node.erzi[j] = node.erzi[j + 1];
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


		void tiaoye(yezi ye) {
			if (qiangyou(ye) == Success) return;
			if (qiangzuo(ye) == Success) return;
			if (hebingyou(ye) == Success) return;
			if (hebingzuo(ye) == Success) return;
			xie(&ye, ye.offset, 1, sizeof(yezi));
		}
		OperationResult qiangyounode(mid node) {
			if (node.par == 0) return Fail;
			mid par;
			du(&par, node.par, 1, sizeof(mid));
			int i = 0;
			for (; i < par.geshu; ++i)
				if (par.erzi[i] == node.offset) break;
			if (i == par.geshu - 1) return Fail;
			mid right;
			du(&right, par.erzi[i + 1], 1, sizeof(mid));
			if (right.geshu <= Mmin) return Fail;

			node.key[node.geshu] = right.key[0];
			node.erzi[node.geshu] = right.erzi[0];
			++node.geshu;
			for (int j = 0; j < right.geshu - 1; ++j) right.key[j] = right.key[j + 1];
			for (int j = 0; j < right.geshu - 1; ++j) right.erzi[j] = right.erzi[j + 1];
			--right.geshu;

			par.key[i + 1] = right.key[0];

			if (node.bz == 1) {
				yezi child;
				du(&child, node.erzi[node.geshu - 1], 1, sizeof(yezi));
				child.par = node.offset;
				xie(&child, child.offset, 1, sizeof(yezi));
			}
			else {
				mid child;
				du(&child, node.erzi[node.geshu - 1], 1, sizeof(mid));
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
				if (par.erzi[i] == node.offset) break;
			if (i == 0) return Fail;
			mid left;
			du(&left, par.erzi[i - 1], 1, sizeof(mid));
			if (left.geshu <= Mmin) return Fail;

			for (int j = node.geshu - 1; j >= 0; --j) node.key[j + 1] = node.key[j];
			for (int j = node.geshu - 1; j >= 0; --j) node.erzi[j + 1] = node.erzi[j];
			node.key[0] = left.key[left.geshu - 1];
			node.erzi[0] = left.erzi[left.geshu - 1];
			++node.geshu;
			--left.geshu;

			par.key[i] = node.key[0];

			if (node.bz == 1) {
				yezi child;
				du(&child, node.erzi[0], 1, sizeof(yezi));
				child.par = node.offset;
				xie(&child, child.offset, 1, sizeof(yezi));
			}
			else {
				mid child;
				du(&child, node.erzi[0], 1, sizeof(mid));
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
				if (par.erzi[i] == node.offset) break;
			if (i == par.geshu - 1) return Fail;
			mid right;
			du(&right, par.erzi[i + 1], 1, sizeof(mid));

			for (int j = 0; j < right.geshu; ++j) {
				node.key[node.geshu] = right.key[j];
				node.erzi[node.geshu] = right.erzi[j];
			}
			if (node.bz == 1) {
				yezi child;
				du(&child, node.erzi[node.geshu], 1, sizeof(yezi));
				child.par = node.offset;
				xie(&child, child.offset, 1, sizeof(yezi));
			}
			else {
				mid child;
				du(&child, node.erzi[node.geshu], 1, sizeof(mid));
				child.par = node.offset;
				xie(&child, child.offset, 1, sizeof(mid));
			}
			++node.geshu;
		}

		for (int j = i + 1; j < par.geshu - 1; ++j)
			par.key[j] = par.key[j + 1], par.erzi[j] = par.erzi[j + 1];
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
				if (par.erzi[i] == node.offset) break;
			if (i == 0) return Fail;
			mid left;
			du(&left, par.erzi[i - 1], 1, sizeof(mid));

			for (int j = 0; j < node.geshu; ++j) {
				left.key[left.geshu] = node.key[j];
				left.erzi[left.geshu] = node.erzi[j];
				if (left.bz == 1) {
					yezi child;
					du(&child, left.erzi[left.geshu], 1, sizeof(yezi));
					child.par = left.offset;
					xie(&child, child.offset, 1, sizeof(yezi));
				}
				else {
					mid child;
					du(&child, left.erzi[left.geshu], 1, sizeof(mid));
					child.par = left.offset;
					xie(&child, child.offset, 1, sizeof(mid));
				}
				++left.geshu;
			}

			for (int j = i; j < par.geshu - 1; ++j)
				par.key[j] = par.key[j + 1], par.erzi[j] = par.erzi[j + 1];
			--par.geshu;
			xie(&left, left.offset, 1, sizeof(mid));
			if (jianchamid(par) == Success) xie(&par, par.offset, 1, sizeof(mid));
			else tiaonode(par);
			return Success;
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
				mid pp;
				du(&pp, par.par, 1, sizeof(mid));
				for (int j = 0; j < pp.geshu; ++j)
					if (pp.erzi[j] == par.offset) {
						pp.erzi[j] = node.offset;
						break;
					}
				node.par = pp.offset;
				xie(&pp, pp.offset, 1, sizeof(mid));
				xie(&node, node.offset, 1, sizeof(mid));
			}
		}
	};
}