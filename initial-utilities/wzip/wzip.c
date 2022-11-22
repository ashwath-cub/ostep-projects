#include <stdio.h>
#include <stdint.h>

#define BLOCK_SIZE 4096     // assumption
#define DATA_UNIT     1     // 1 byte

typedef enum 
{
    MORE_DATA_EXISTS=0,
    NO_MORE_DATA    
}file_read_status_t;

typedef struct
{
    uint32_t length;
    char encoding;
}rle_data_type;

uint8_t block_data[ BLOCK_SIZE ];
uint8_t rle_compressed_block[ BLOCK_SIZE ];

int read_block_from_file(FILE* file_descriptor, uint32_t block_size, uint8_t* data_buffer);

int main (int argc, char** argv)
{
    printf("%d argument(s) recevied: ", argc);
    int index;
    for(index=0; index<argc; index++)
    {
        printf("%s  ");
    }
    FILE* file_descriptor = fopen(argv[0], "r");

    if( file_descriptor == NULL)
    {
        perror("file %s could not be opened\n", argv[0]);
        exit();
    }

    compress_file( file_descriptor )

}

int compress_file(FILE* file_descriptor )
{
    file_read_status_t read_status; 
    do
    { 
        read_status = read_block_from_file( file_descriptor, BLOCK_SIZE, &block_data );

    }while( read_status == MORE_DATA_EXISTS );
}

int compress_block_using_rle( uint8_t* input_data_buffer, uint8_t* rle_output_block, uint32_t block_size, rle_data_type* last_entry)
{
    uint16_t input_data_buffer_index =0;
    rle_data_type next_rle_entry = {0};

    if( last_entry != NULL )
    {
        if ( last_entry->encoding == input_data_buffer[input_data_buffer_index] )
        {
            next_rle_entry->encoding = last_entry->encoding ;

            do
            {
                next_rle_entry->length = last_entry->length + input_data_buffer_index + 1;
                input_data_buffer_index ++ ;
            }while( ( last_entry->encoding == input_data_buffer[ input_data_buffer_index ] ) && ( input_data_buffer_index < block_size ) );
        }
        
    }



    return 0;
}

file_read_status_t read_block_from_file( FILE* file_descriptor, uint32_t block_size, uint8_t* data_buffer)
{
    int fread_return = fread( (void*)data_buffer, DATA_UNIT, block_size, file_descriptor );
    file_read_status_t read_status;

    if( fread_return == 0 )
    {
        if( ferror( file_descriptor ) !=0 )
        {
            perror("ran into error reading file\n");
        }
        else
        {
            read_status = NO_MORE_DATA;
        }
    }
    else if( fread_return < block_size )
    {
        read_status = NO_MORE_DATA;
    }
    else if( fread_return == block_size )
    {
        read_status = MORE_DATA_EXISTS ;
    }
    else
    {
        printf("fread_return:%d is greater than the block_size", fread_return);
        exit();
    }


}


