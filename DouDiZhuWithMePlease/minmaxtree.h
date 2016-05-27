#ifndef __F__MYPROJECTS_DOUDIZHUWITHMEPLEASE__DOUDIZHUWITHMEPLEASE_MINMAXTREE_H__
#define __F__MYPROJECTS_DOUDIZHUWITHMEPLEASE__DOUDIZHUWITHMEPLEASE_MINMAXTREE_H__

/************************************************************************/
/* 只寻找可以得到决策的一种方法，其他情况剪枝不再处理                   */
/* 方便起见，本树只增加节点不减少节点，由节点自己释放自己和孩子			*/
/************************************************************************/

#include <deque>
#include <vector>

// 最大最小树 - 节点
class MinMaxTreeNodeBase
{
public:
	MinMaxTreeNodeBase() :m_father(NULL) {};
	virtual ~MinMaxTreeNodeBase(){
		for( auto it = m_vec_children.begin(); it != m_vec_children.end(); ++it ) { delete (*it); }
		m_vec_children.clear();
	};

	void AddChildren( MinMaxTreeNodeBase* p_children ){
		m_vec_children.push_back( p_children );
		p_children->SetFather( this );
	}
	void SetFather( MinMaxTreeNodeBase* p_father ){ m_father = p_father; }
	std::vector< MinMaxTreeNodeBase* >::iterator FirstChild(){ return m_vec_children.begin(); }
	std::vector< MinMaxTreeNodeBase* >::iterator LastChild(){ return m_vec_children.end(); }

	virtual void Print() = 0;
	virtual void GenerateChildren() = 0;
	virtual bool isUseful(){ return m_curIsMax; }
	virtual bool isAlreadyMinMax(){ return m_isNeedAllMax == m_curIsMax;} // 因为这里的 isNeed 是对自己的孩子而言；所以此处对自己是相反的；所以应该是相等。

	bool m_isNeedAllMax;
	bool m_curIsMax;

protected:
	std::vector< MinMaxTreeNodeBase* > m_vec_children;
	MinMaxTreeNodeBase* m_father;

};

// 最大最小树 - 主体
template < typename TreeNode >
class MinMaxTree
{
public:
	typedef TreeNode* TreeNodePtr;
	MinMaxTree():m_root(NULL){}
	virtual ~MinMaxTree(){
		delete m_root;
		m_root = NULL;
	}

	void AddNode( TreeNodePtr father, TreeNodePtr children ){
		father->AddChildren( children );
	}

protected:
	TreeNodePtr m_root;
};

template < typename TreeNode >
class DecidingTree : public MinMaxTree< TreeNode >
{
public:
	DecidingTree(){};
	virtual ~DecidingTree(){};

	void StartGenerate(){
		m_root->GenerateChildren();
	}

	void Print(){
		std::deque< MinMaxTreeNodeBase* > temp_stack;
		temp_stack.push_back( m_root );

		while( !temp_stack.empty() )
		{
			MinMaxTreeNodeBase* cur_node = temp_stack.front();
			temp_stack.pop_front();

			if( !cur_node->isUseful() )
			{
				continue;
			}

			cur_node->Print();
			for( auto it = cur_node->FirstChild(); it != cur_node->LastChild(); ++it )
			{
				if( (*it)->isUseful() )
				{
					temp_stack.push_back( (*it) );
					break;
				}
			}
		}
	}
};

#endif 