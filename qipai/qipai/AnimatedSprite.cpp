#include "AnimatedSprite.hpp"
#include "Animation.hpp"

#include <cassert>
//����������

AnimatedSprite::FuncType AnimatedSprite::defaultFunc = []()->void{};
//���캯��ֻ��ʼ�����в�ͬ���Ե���ȷֵ:
AnimatedSprite::AnimatedSprite(Animation* animation,Status status,const sf::Time& deltaTime,bool loop,int repeat) : on_finished(defaultFunc),_delta(deltaTime),_loop(loop), _repeat(repeat),_status(status)
{
    setAnimation(animation); 
}
//�������ֻ�ڵ�ǰ������ͬ������²Ż�ı��µ���������֡����Ϊ�¶����ĵ�һ��֡��ע�⣬������һ��֡����洢����Ϊ�������յ��¶����С�
void AnimatedSprite::setAnimation(Animation* animation)
{
    if(_animation != animation)
    {
        _animation = animation;
        _elapsed = sf::Time::Zero;
        _currentFrame = 0;
        setFrame(0,true);
    }
}

Animation* AnimatedSprite::getAnimation()const
{
    return _animation;
}

void AnimatedSprite::setFrameTime(sf::Time deltaTime)
{
    _delta = deltaTime;
}

sf::Time AnimatedSprite::getFrameTime()const
{
    return _delta;
}

void AnimatedSprite::setLoop(bool loop)
{
    _loop = loop;
}

bool AnimatedSprite::getLoop()const
{
    return _loop;
}
void AnimatedSprite::setRepeate(int nb)
{
    _repeat = nb;
}

int AnimatedSprite::getRepeate()const
{
    return _repeat;
}

void AnimatedSprite::play()
{
    _status = Playing;
}
void AnimatedSprite::pause()
{
    _status = Paused;
}

void AnimatedSprite::stop()
{
    _status = Stopped;
    _currentFrame = 0;
    setFrame(0,true);
}

AnimatedSprite::Status AnimatedSprite::getStatus()const
{
    return _status;
}
//�˺�������ǰ֡����Ϊ���ڲ��������ȡ����֡��
void AnimatedSprite::setFrame(size_t index)
{
    assert(_animation);
    _currentFrame = index % _animation->size();
    setFrame(_currentFrame,true);
}
//�˺����ı���ʾͼ�����ɫ���롣Ϊ�ˣ����ǽ�ÿ���ڲ��������ɫ����Ϊ��Ϊ�������յ�����ɫ:
void AnimatedSprite::setColor(const sf::Color& color)
{
    _vertices[0].color = color;
    _vertices[1].color = color;
    _vertices[2].color = color;
    _vertices[3].color = color;
}
/////////////////////////////////////////////////////////////////////
//���Ĺ������ڴﵽʱ������ʱ�ӵ�ǰ֡�л�����һ֡��һ�����ǵ��ﶯ�������һ֡�����������������:
//(1)����_loopֵ�����õ�һ������֡���в���
//(2)���_repeatֵ��Ȩ���������������õ�һ������֡���в���
//(3)��������������£�����ͨ�������ڲ��ص���������on fnish���¼�
void AnimatedSprite::update(const sf::Time& deltaTime)
{
    if(_status == Playing && _animation)
    {
        _elapsed += deltaTime;
        if(_elapsed > _delta)
        {//֡�л�
            _elapsed -= _delta;
            if(_currentFrame + 1 < _animation->size())
                ++_currentFrame;
            else
            {//�Ѿ������һ֡
                _currentFrame = 0;
                if(!_loop)
                {//�Ƿ�ʼ��һ��ѭ��?
                    --_repeat;
                    if(_repeat<=0)
                    {//no, so we stop
                        _status = Stopped;
                        on_finished();
                    }
                }
            }
        }
		//update the frame
        setFrame(_currentFrame,false);
    }
}
//�ú�����Ŀ���ǽ���ͬ��������Ը���Ϊ�ڲ�����������ԣ���λ�ú��������꣩
void AnimatedSprite::setFrame(size_t index,bool resetTime)
{
    if(_animation)
    {
        sf::IntRect rect = _animation->getRect(index);

        _vertices[0].position = sf::Vector2f(0.f, 0.f);
        _vertices[1].position = sf::Vector2f(0.f, static_cast<float>(rect.height));
        _vertices[2].position = sf::Vector2f(static_cast<float>(rect.width), static_cast<float>(rect.height));
        _vertices[3].position = sf::Vector2f(static_cast<float>(rect.width), 0.f);

        float left = static_cast<float>(rect.left);
        float right = left + static_cast<float>(rect.width);
        float top = static_cast<float>(rect.top);
        float bottom = top + static_cast<float>(rect.height);

        _vertices[0].texCoords = sf::Vector2f(left, top);
        _vertices[1].texCoords = sf::Vector2f(left, bottom);
        _vertices[2].texCoords = sf::Vector2f(right, bottom);
        _vertices[3].texCoords = sf::Vector2f(right, top);
    }

    if(resetTime)
        _elapsed = sf::Time::Zero;
}
//��Ϊ����̳���sf::Transformable������������Ҫ���ǿ��ܵļ��α任����������Ҫʹ�õ����������ջ����ڲ���������
void AnimatedSprite::draw(sf::RenderTarget& target,sf::RenderStates states) const
{
    if (_animation && _animation->_texture)
    {
        states.transform *= getTransform();
        states.texture = _animation->_texture;
        target.draw(_vertices, 4, sf::Quads, states);
    }
}