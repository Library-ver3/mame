#pragma once

#include <vector>
#include <stack>
#include <map>
#include "BehaviorTree.h"

template <class T>
class NodeBase;

// Behavior�ۑ��f�[�^
template <class T>
class BehaviorData
{
public:
    BehaviorData() { Initialize(); }

    // ������
    void Initialize();

    // �V�[�P���X�m�[�h�̃v�b�V��
    void PushSequenceNode(NodeBase<T>* node) { sequenceStack_.push(node); }

    // �V�[�P���X�m�[�h�̃|�b�v
    NodeBase<T>* PopSequenceNode();

public:// �擾�E�ݒ�
    int GetSequenceStep(std::string name);
    void SetSequenceStep(std::string name, int step);

private:
    std::stack<NodeBase<T>*> sequenceStack_;            // ���s���钆�ԃm�[�h���X�^�b�N
    std::map<std::string, int> runSequenceStepMap_;  // ���s���̒��ԃm�[�h�̃X�e�b�v���L�^
};