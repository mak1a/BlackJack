
# pragma once
# include "Common.hpp"

// �Q�[���V�[��
class Game : public MyApp::Scene {
private:
    /// <summary>
    /// �g�����v�̕`��N���X
    /// </summary>
    PlayingCard::Pack m_pack;

    /// <summary>
    /// �^�C�}�[
    /// </summary>
    Stopwatch m_distributeTimer, m_distributeEnemyTimer;

    /// <summary>
    /// �g�����v�N���X
    /// </summary>
    // �g�����v�̃N���X
    Array<PlayingCard::Card> m_playerCards, m_enemyCards;

    /// <summary>
    /// �q�b�g�A�X�^���h��I�ԃ{�^��
    /// </summary>
    RectF m_hitButton, m_standButton;

    /// <summary>
    /// �ǂ���̃^�[����
    /// </summary>
    enum class Turn {
        Distribute,	// ���҂ɃJ�[�h��2�����z��
        Player,	// Player�̃^�[��
        Enemy,	// Enemy�̃^�[��
        Result	// ���ʕ\��
    } m_turn;

    enum class SelectButton {
        Hit,
        Stand,
        None
    } m_select;

    /// <summary>
    /// A�������Ă��閇��
    /// </summary>
    uint32 m_countAce, m_countAceEnemy;

    /// <summary>
    /// �_��
    /// </summary>
    uint32 m_playerScore, m_enemyScore;

    /// <summary>
    /// �q����
    /// �V�[���ړ����ɐ������ς��Ȃ��悤�ɂ���
    /// </summary>
    const uint32 m_betMoney;

    /// <summary>
    /// ������
    /// �V�[���ړ����ɐ������ς��Ȃ��悤�ɂ���
    /// </summary>
    const uint32 m_haveMoney;

    /// <summary>
    /// ������
    /// �V�[���ړ����ɐ������ς��Ȃ��悤�ɂ���
    /// </summary>
    const uint32 m_winCount;

    /// <summary>
    /// �s�k��
    /// �V�[���ړ����ɐ������ς��Ȃ��悤�ɂ���
    /// </summary>
    const uint32 m_loseCount;

    /// <summary>
    /// ����������
    /// �V�[���ړ����ɐ������ς��Ȃ��悤�ɂ���
    /// </summary>
    const uint32 m_drawCount;

    /// <summary>
    /// Player������������
    /// </summary>
    /// <returns>Player������������</returns>
    [[nodiscard]] bool IsPlayerWin() const noexcept {
        if (m_playerCards.size() == 2) {
            if (((m_playerCards[0].isAce() && m_playerCards[1].rank >= 10) || (m_playerCards[1].isAce() && m_playerCards[0].rank >= 10))
                && !((m_enemyCards[0].isAce() && m_enemyCards[1].rank >= 10) || (m_enemyCards[1].isAce() && m_enemyCards[0].rank >= 10))) {
                return true;
            }
        }

        if (m_enemyCards.size() == 2) {
            if (((m_enemyCards[0].isAce() && m_enemyCards[1].rank >= 10) || (m_enemyCards[1].isAce() && m_enemyCards[0].rank >= 10))
                && !((m_playerCards[0].isAce() && m_playerCards[1].rank >= 10) || (m_playerCards[1].isAce() && m_playerCards[0].rank >= 10))) {
                return false;
            }
        }

        return (m_playerScore <= 21 && (m_playerScore > m_enemyScore || m_enemyScore > 21));
    }
    /// <summary>
    /// ����������
    /// </summary>
    /// <returns>����������</returns>
    [[nodiscard]] bool IsDrawGame() const noexcept {
        return m_playerScore <= 21 && m_playerScore == m_enemyScore;
    }

    /// <summary>
    /// �v���C���[�ɃJ�[�h��z�鏈��
    /// </summary>
    void DistributeChild();

    /// <summary>
    /// �G�ɃJ�[�h��z�鏈��
    /// </summary>
    void DistributeParent();

    /// <summary>
    /// �Q�[���J�n����ɗ��҂ɔz�鏈��
    /// �_���Ȃǂ�����������
    /// </summary>
    void DistributeInit();

    /// <summary>
    /// �v���C���[�̃g�����v�̍��v�l���v�Z���鏈��
    /// </summary>
    void CalucuratePlayerScore();

    /// <summary>
    /// �G�l�~�[�̃g�����v�̍��v�l���v�Z���鏈��
    /// </summary>
    void CalucurateEnemyScore();

    /// <summary>
    /// ��ɏo�Ă�g�����v���������
    /// �����ɓq�����̉���Ȃǂ�����
    /// </summary>
    void CollectCard();

public:
    Game(const InitData& init);

    void update() override;

    void draw() const override;
};
