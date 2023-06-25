#include "buffer_string.h"

#include <cstring>
#include <fstream>

#include "utils.h"

void Buffer_string::clear()
{
    capacity = 0;
    for (size_t i = 0; i < MAX_BUFFER_SIZE - 1; i++)
    {
        text[i] = '\0';
    }
}

void Buffer_string::copy_from(const Buffer_string &src)
{
    clear();

    size_t index{0};
    if (src.capacity < MAX_BUFFER_SIZE)
    {
        capacity = src.capacity;
        for (; index < src.capacity; index++)
        {
            text[index] = src.text[index];
        }
    }
    else // Shouldn't happen
    {
        capacity = MAX_BUFFER_SIZE - 1;
        for (; index < MAX_BUFFER_SIZE - 2; index++)
        {
            text[index] = src.text[index];
        }
    }
    text[MAX_BUFFER_SIZE - 1] = '\0'; // failsafe
}

void Buffer_string::copy_from(const char *src_str)
{
    clear();

    if (src_str != nullptr)
    {
        capacity = min(strlen(src_str), MAX_BUFFER_SIZE - 1);
        size_t index{0};
        if (strlen(src_str) < MAX_BUFFER_SIZE - 1)
        {
            for (; index < strlen(src_str); index++)
            {
                text[index] = src_str[index];
            }
        }
        else
        {
            for (; index < MAX_BUFFER_SIZE - 1; index++)
            {
                text[index] = src_str[index];
            }
        }

        text[MAX_BUFFER_SIZE - 1] = '\0'; // failsafe
    }
}

Buffer_string Buffer_string::remove_quotes()
{
    if (!is_quoted())
    {
        return *this;
    }

    Buffer_string dest;
    for (size_t i = 1; i < capacity - 1; i++)
    {
        dest.append(text[i]);
    }

    return dest;
}

Buffer_string Buffer_string::add_quotes()
{
    if (is_quoted())
    {
        return *this;
    }

    Buffer_string dest;

    dest.append('"');
    for (size_t i = 0; i < capacity; i++)
    {
        dest.append(text[i]);
    }
    dest.append('"');

    return dest;
}

Buffer_string Buffer_string::reverse() const
{
    Buffer_string dest;

    for (size_t i = 0; i < capacity; i++)
    {
        dest.append(text[capacity - i - 1]);
    }

    return dest;
}

Buffer_string::Buffer_string() : capacity(0) { clear(); }

Buffer_string::Buffer_string(const char *src_str)
{
    copy_from(src_str);
}

Buffer_string::Buffer_string(const Buffer_string &src) : capacity(src.capacity)
{
    copy_from(src);
}

char Buffer_string::get_char_at_index(size_t index) const
{
    return text[index];
}

Buffer_string Buffer_string::mod_at_index(size_t index, const char value)
{
    if (index >= 0 && index < capacity && value != '\0')
    {
        text[index] = value;
    }

    return *this;
}

Buffer_string Buffer_string::append(char value)
{
    if (capacity < MAX_BUFFER_SIZE - 2 && value != '\0')
    {
        text[capacity++] = value;
        text[capacity] = '\0';
    }
    text[MAX_BUFFER_SIZE - 1] = '\0'; // failsafe

    return *this;
}

size_t Buffer_string::get_capacity() const
{
    return capacity;
}

const char *Buffer_string::get_text_ptr() const
{
    return text;
}

Buffer_string Buffer_string::trim_whitespaces() const
{
    if (capacity <= 0)
    {
        return *this;
    }

    size_t start{0};
    while (text[start] == ' ' || text[start] == '\t')
    {
        ++start;
    }

    size_t end{capacity};
    while (text[end] == '\0' || text[end] == ' ' || text[end] == '\t')
    {
        --end;
    }

    Buffer_string dest;
    for (size_t i = start; i <= end; i++)
    {
        dest.append(text[i]);
    }

    dest.capacity = end - start + 1;

    return dest;
}

bool Buffer_string::is_empty() const
{
    for (size_t i = 0; i < MAX_BUFFER_SIZE; i++)
    {
        if (text[i] != '\0')
        {
            return 0;
        }
    }

    return 1;
}

bool Buffer_string::is_whole() const
{
    for (size_t i = 0; i < capacity; i++)
    {
        if (text[i] < '0' || text[i] > '9')
        {
            return 0;
        }
    }

    return 1;
}

bool Buffer_string::is_decimal() const
{
    if (is_empty())
    {
        return 0;
    }

    size_t index{0};
    if (text[index] == '-' || text[index] == '+')
    {
        index++;
    }

    size_t dot_count{0};
    size_t digit_count{0};
    while (text[index] != '\0' && dot_count <= 1)
    {
        if (text[index] == '.')
        {
            dot_count++;
        }
        else if (text[index] >= '0' && text[index] <= '9')
        {
            digit_count++;
        }
        else
        {
            return false;
        }

        index++;
    }

    return dot_count == 1 && digit_count > 0;
}

bool Buffer_string::is_quoted() const
{
    return text[0] == '"' && text[capacity - 1] == '"' && capacity >= 2;
}

long Buffer_string::to_whole() const
{
    if (!is_whole())
    {
        return 0;
    }

    size_t index{0};
    bool negative{text[0] == '-'};
    long long result{0};

    if (text[index] == '-' || text[index] == '+')
    {
        index++;
    }

    while (text[index] != '\0')
    {
        result = result * 10 + (text[index++] - '0');
    }

    return negative ? -result : result;
}

double Buffer_string::to_decimal() const
{
    if (!is_decimal())
    {
        return 0.0;
    }

    size_t index{0};
    size_t digits_after_dot{0};
    bool negative{text[0] == '-'};
    bool dot_met{0};
    double result{0};

    if (text[index] == '-' || text[index] == '+')
    {
        index++;
    }

    while (text[index] != '\0')
    {
        if (text[index] != '.')
        {
            result = result * 10.0 + (text[index] - '0');
            if (dot_met)
            {
                digits_after_dot++;
            }
        }
        else
        {
            dot_met = 1;
        }
        index++;
    }

    for (size_t i = 0; i < digits_after_dot; i++)
    {
        result /= 10.0;
    }

    return negative ? -result : result;
}

Buffer_string Buffer_string::operator=(const Buffer_string &src)
{
    if (this != &src)
    {
        clear();
        copy_from(src);
    }

    return *this;
}

const char Buffer_string::operator[](size_t index) const
{
    if (index >= 0 || index < capacity)
    {
        return text[index];
    }
    else
    {
        return '\0';
    }
}

bool Buffer_string::operator==(const char *compare_to) const
{
    if (strlen(compare_to) != capacity)
    {
        return false;
    }

    for (size_t i = 0; i < strlen(compare_to) && i < capacity; i++)
    {
        if (compare_to[i] != text[i])
        {
            return false;
        }
    }

    return true;
}

bool Buffer_string::operator==(const Buffer_string &compare_to) const
{

    if (compare_to.capacity != capacity)
    {
        return false;
    }

    for (size_t i = 0; i < compare_to.capacity && i < capacity; i++)
    {
        if (compare_to[i] != text[i])
        {
            return false;
        }
    }

    return true;
}

bool Buffer_string::operator!=(const char *compare_to) const
{
    return !(*this == compare_to);
}

bool Buffer_string::operator!=(const Buffer_string &compare_to) const
{
    return !(*this == compare_to);
}

std::ostream &operator<<(std::ostream &stream_out, const Buffer_string &bff)
{
    stream_out << bff.text;
    return stream_out;
}

Buffer_string Buffer_string::getline(std::istream &stream_in)
{
    clear();
    stream_in.getline(text, MAX_BUFFER_SIZE);
    size_t index{0};
    while (text[index] != '\0' && index < MAX_BUFFER_SIZE - 1)
    {
        capacity++;
        index++;
    }
    text[MAX_BUFFER_SIZE - 1] = '\0';

    *this = this->trim_whitespaces();
    return *this;
}