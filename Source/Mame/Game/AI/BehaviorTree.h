#pragma once
#include <string>

template <class T>
class ActionBase;
template <class T>
class JudgmentBase;
template <class T>
class NodeBase;
template <class T>
class BehaviorData;

template <class T>
class BehaviorTree
{
public:// �萔
    // �I�����[��
    enum class SelectRule
    {
        Non,
        Priority,           // �D�揇��
        Sequence,           // �V�[�N�G���X
        SequentialLooping,  // �V�[�N�G���V�������[�s���O
        Random,             // �����_��
    };

public:
    BehaviorTree() : root_(nullptr), owner_(nullptr) {}
    BehaviorTree(T* enemy) : root_(nullptr), owner_(enemy) {}
    ~BehaviorTree();

    // ���s�m�[�h�𐄘_����
    NodeBase<T>* ActiveNodeInference(BehaviorData<T>* data);

    // �V�[�N�G���X�m�[�h�󐄘_�J�n
    NodeBase<T>* SequenceBack(NodeBase<T>* sequenceNode, BehaviorData<T>* data);

    // �m�[�h�ǉ�
    void AddNode(
        std::string parentName,     // �e�m�[�h�����w��B�������ǂ̃m�[�h�ɕR�Â����A�܂��Z��m�[�h�̒T���Ɏg�p
        std::string entryName,      // �����̃m�[�h���B���s�m�[�h�̒T���ȂǂɎg�p
        int priority,               // �D�揇�ʁB���[���� Priority �Ȃǂ̏ꍇ�Ɏg�p
        SelectRule selectRule,      // �e���ԃm�[�h�̃��[�����w��
        JudgmentBase<T>* judgment,  // �m�[�h�����s�ł��邩���f����I�u�W�F�N�g
        ActionBase<T>* action       // ���ۂɍs�������̃I�u�W�F�N�g
    );

    // ���s
    NodeBase<T>* Run(NodeBase<T>* actionNode, BehaviorData<T>* data, float elapsedTime);

private:
    // �m�[�h�S�폜
    void NodeAllClear(NodeBase<T>* delNode);

private:
    // ���[�g�m�[�h
    NodeBase<T>* root_;
    T* owner_;
};

