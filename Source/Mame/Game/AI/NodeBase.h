#pragma once

#include <vector>
#include <string>
#include "BehaviorTree.h"
#include "ActionBase.h"

template <class T>
class JudgmentBase;
template <class T>
class BehaviorData;

// ���������[�N�����p
#define debug_new new(_NORMAL_BLOCK, __FILE__, __LINE__)

// �m�[�h
template <class T>
class NodeBase
{
public:
    NodeBase(
        const std::string& name,
        NodeBase<T>* parent,
        NodeBase<T>* sibling,
        const unsigned int& priority,
        const BehaviorTree::SelectRule& selectRule,
        JudgmentBase<T>* judgment,
        ActionBase<T>* action,
        const int& hierarchyNo
    )
        :name_(name), parent_(parent), sibling_(sibling), priority_(priority),
        selectRule_(selectRule), judgment_(judgment), action_(action),
        hierarchyNo_(hierarchyNo), children_(NULL)
    {
    }
    ~NodeBase();

public:// �擾�E�ݒ�
#pragma region [Set,Get..etc]Function
    // ----- ���O -----
    const std::string GetName() const { return name_; }

    // ----- �e�m�[�h -----
    NodeBase<T>* GetParent() const { return parent_; }
    void SetParent(NodeBase<T>* parent) { parent_ = parent; }

    // ----- �q�m�[�h -----
    NodeBase<T>* GetChild(const int index) const;
    NodeBase<T>* GetLastChild() const;   // ( ���� )
    NodeBase<T>* GetTopChild() const;    // ( �擪 )

    void AddChild(NodeBase<T>* child) { children_.push_back(child); }

    // ----- �Z��m�[�h -----
    NodeBase<T>* GetSibling() const { return sibling_; }
    void SetSibling(NodeBase<T>* sibling) { sibling_ = sibling; }

    // �K�w�ԍ�
    const int GetHirerchyNo() const { return hierarchyNo_; } 
    
    // �D�揇��
    const int GetPriority() const { return priority_; }

    // �s���f�[�^�������Ă��邩
    const bool HasAction() const { return action_ != nullptr ? true : false; }
#pragma endregion// [Set,Get..etc]Function

    // ���s�۔���
    const bool Judgment() const;

    // �D�揇�ʑI��
    NodeBase<T>* SelectPriority(std::vector<NodeBase<T>*>* list);

    // �����_���I��
    NodeBase<T>* SelectRandom(std::vector<NodeBase<T>*>* list);

    // �V�[�P���X�I��
    NodeBase<T>* SelectSequence(std::vector<NodeBase<T>*>* list, BehaviorData<T>* data);

    // �m�[�h����
    NodeBase<T>* SearchNode(const std::string& searchName);

    // �m�[�h���_
    NodeBase<T>* Inference(BehaviorData<T>* data);

    // ���s
    ActionBase::State Run(const float& elapsedTime)
    {
        // action�����邩���f�B����΃����o�֐�Run()���s�������ʂ�Ԃ�
        if (action_ != nullptr)
        {
            return action_->Run(elapsedTime);
        }

        return ActionBase::State::Failed;
    }

public:
    std::vector<NodeBase*> children_;

protected:
    std::string                 name_;          // ���O
    BehaviorTree::SelectRule selectRule_;    // �I�����[��
    JudgmentBase<T>*            judgment_;      // ����N���X
    ActionBase<T>*              action_;        // ���s�N���X
    unsigned int                priority_;      // �D�揇��
    NodeBase<T>*                parent_;        // �e�m�[�h
    NodeBase<T>*                sibling_;       // �Z��m�[�h
    int                         hierarchyNo_;   // �K�w�ԍ�
};