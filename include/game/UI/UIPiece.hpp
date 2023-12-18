#ifndef PIECE_INCLUDE
#define PIECE_INCLUDE

#include "UIElement.hpp"
#include "UIManager.hpp"

#include <map>
#include <string>

enum class PieceType
{
    Bit = 0b100000000,
    ForwardL = 0b111100100,
    SmallForwardL = 0b110100000,
    BackwardL = 0b100100111,
    SmallBackwardL = 0b100110000,
    ThreeByThree = 0b111111111,
    TwoByTwo = 0b110110000,
    Donut = 0b111101111,
    VerticalBar = 0b111000000,
    SmallVerticalBar = 0b110000000,
    HorizontalBar = 0b100100100,
    SmallHorizontalBar = 0b100100000
};

class UIPiece : public UIElement
{
private:
    inline static std::map<PieceType, std::string> _pieceTextureFromType =
        {
            {PieceType::Bit, "bit"},
            {PieceType::ForwardL, "forward_l"},
            {PieceType::SmallForwardL, "small_forward_l"},
            {PieceType::BackwardL, "backward_l"},
            {PieceType::SmallBackwardL, "small_backward_l"},
            {PieceType::ThreeByThree, "three_by_three"},
            {PieceType::TwoByTwo, "two_by_two"},
            {PieceType::Donut, "donut"},
            {PieceType::VerticalBar, "vertical_bar"},
            {PieceType::SmallVerticalBar, "small_vertical_bar"},
            {PieceType::HorizontalBar, "horizontal_bar"},
            {PieceType::SmallHorizontalBar, "small_horizontal_bar"}};

    inline static std::vector<PieceType> pieceTypes =
        {PieceType::Bit,
         PieceType::ForwardL,
         PieceType::SmallForwardL,
         PieceType::BackwardL,
         PieceType::SmallBackwardL,
         PieceType::ThreeByThree,
         PieceType::TwoByTwo,
         PieceType::Donut,
         PieceType::VerticalBar,
         PieceType::SmallVerticalBar,
         PieceType::HorizontalBar,
         PieceType::SmallHorizontalBar};

    PieceType _pieceType;

    Vector2 _startPos;

    bool _isDragging = false;

public:
    UIPiece();
    bool init(Vector2 pos, Vector2 dims);
    void update() override;
    void reset();
};

#endif